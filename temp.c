#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_PROCS 32
#define MAX_QUEUE 32

typedef struct {
    int total_ram;
    int available_ram;
    int total_cores;
    int available_cores;
    int total_hdd;
    int available_hdd;
} system_resources_t;

typedef struct {
    const char *name;
    int ram_required;
    int cores_required;
    int hdd_required;
} app_requirements_t;

typedef struct {
    pid_t pid;
    app_requirements_t *req;
} proc_entry_t;

typedef struct {
    app_requirements_t *req;
} queued_proc_t;

// ---- Globals ----
static system_resources_t sys_resources;
static app_requirements_t apps[] = {
    {"./calculator",  512, 1, 100},
    {"./createFile",  256, 1,  50},
    {"./deleteFile",  256, 1,  50},
    {"./fileInfo",    256, 1,  50},
    {"./guessNumber", 256, 1,  50},
    {"./notePad",    1024, 2, 200},
    {"./tiktak",      512, 1, 100},
    {"./time",        256, 1,  50}
};
static proc_entry_t proc_table[MAX_PROCS];
static int proc_count = 0;

// Queue
static queued_proc_t queue[MAX_QUEUE];
static int queue_front = 0, queue_rear = 0;

// Semaphore protecting sys_resources + queue + proc_table
static sem_t resource_sem;

// Monitor thread handle
static pthread_t monitor_thread;

// ---- Helpers ----
void display_remaining_resources() {
    printf("\nAvailable RAM: %d/%d MB (%.1f%%)\n",
        sys_resources.available_ram, sys_resources.total_ram,
        sys_resources.available_ram * 100.0 / sys_resources.total_ram);
    printf("Available CPU cores: %d/%d (%.1f%%)\n",
        sys_resources.available_cores, sys_resources.total_cores,
        sys_resources.available_cores * 100.0 / sys_resources.total_cores);
    printf("Available HDD: %d/%d MB (%.1f%%)\n",
        sys_resources.available_hdd, sys_resources.total_hdd,
        sys_resources.available_hdd * 100.0 / sys_resources.total_hdd);
}

int can_allocate(app_requirements_t *req) {
    return sys_resources.available_ram   >= req->ram_required &&
           sys_resources.available_cores >= req->cores_required &&
           sys_resources.available_hdd   >= req->hdd_required;
}

void enqueue(app_requirements_t *req) {
    if ((queue_rear + 1) % MAX_QUEUE == queue_front) {
        printf("Queue full, cannot enqueue process.\n");
        return;
    }
    queue[queue_rear].req = req;
    queue_rear = (queue_rear + 1) % MAX_QUEUE;
    printf("Process '%s' added to queue.\n", req->name);
}

int is_queue_empty() {
    return queue_front == queue_rear;
}

app_requirements_t* dequeue() {
    if (is_queue_empty()) return NULL;
    app_requirements_t *req = queue[queue_front].req;
    queue_front = (queue_front + 1) % MAX_QUEUE;
    return req;
}

void display_boot_screen() {
    system("clear");
    printf("\n\n");
    printf("#################################################\n");
    printf("#       W E L C O M E   T O   N O V A           #\n");
    printf("#################################################\n");
    time_t now = time(NULL);
    char buf[64];
    strftime(buf, sizeof(buf), "%A, %d %B %Y %I:%M:%S %p", localtime(&now));
    printf("\nCurrent Time: %s\n", buf);
    sleep(2);
    system("clear");
}

// ---- Core scheduling from queue (now semaphore‐protected) ----
void try_schedule_from_queue() {
    sem_wait(&resource_sem);

    int loop_count = (queue_rear - queue_front + MAX_QUEUE) % MAX_QUEUE;
    for (int i = 0; i < loop_count; i++) {
        app_requirements_t *req = dequeue();
        if (!req) break;

        if (can_allocate(req)) {
            printf("Scheduling queued process: %s\n", req->name);
            // allocate
            sys_resources.available_ram   -= req->ram_required;
            sys_resources.available_cores -= req->cores_required;
            sys_resources.available_hdd   -= req->hdd_required;

            pid_t pid = fork();
            if (pid == 0) {
                execlp("gnome-terminal", "gnome-terminal", "--wait", "--", req->name, NULL);
                perror("execlp");
                _exit(1);
            } else if (pid > 0) {
                proc_table[proc_count].pid = pid;
                proc_table[proc_count].req = req;
                proc_count++;
                display_remaining_resources();
            } else {
                perror("fork");
                // rollback
                sys_resources.available_ram   += req->ram_required;
                sys_resources.available_cores += req->cores_required;
                sys_resources.available_hdd   += req->hdd_required;
                enqueue(req);
            }
        } else {
            // Not enough resources → requeue and stop trying
            enqueue(req);
            break;
        }
    }

    sem_post(&resource_sem);
}

// ---- Monitor thread: reaps + then reschedules ----
void *monitor_fn(void *arg) {
    while (1) {
        sem_wait(&resource_sem);
        // reap any children
        int status;
        pid_t pid;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            // find entry
            for (int i = 0; i < proc_count; i++) {
                if (proc_table[i].pid == pid) {
                    // release
                    app_requirements_t *r = proc_table[i].req;
                    sys_resources.available_ram   += r->ram_required;
                    sys_resources.available_cores += r->cores_required;
                    sys_resources.available_hdd   += r->hdd_required;
                    printf("\nProcess %d exited, resources reclaimed.\n", pid);
                    display_remaining_resources();
                    // remove from table
                    memmove(&proc_table[i], &proc_table[i+1],
                            sizeof(proc_entry_t) * (proc_count - i - 1));
                    proc_count--;
                    break;
                }
            }
        }
        // then try scheduling any queued jobs
        try_schedule_from_queue();
        sem_post(&resource_sem);
        sleep(1);
    }
    return NULL;
}

// ---- Launch new app (also semaphore‐protected) ----
void launch_app(const char *app_name) {
    // find req
    app_requirements_t *req = NULL;
    int n = sizeof(apps)/sizeof(apps[0]);
    for (int i = 0; i < n; i++)
        if (strcmp(apps[i].name, app_name) == 0)
            req = &apps[i];
    if (!req) {
        printf("Application not found\n");
        return;
    }

    sem_wait(&resource_sem);
    if (can_allocate(req)) {
        // allocate
        sys_resources.available_ram   -= req->ram_required;
        sys_resources.available_cores -= req->cores_required;
        sys_resources.available_hdd   -= req->hdd_required;
        display_remaining_resources();

        pid_t pid = fork();
        if (pid == 0) {
            execlp("gnome-terminal", "gnome-terminal", "--wait", "--", req->name, NULL);
            perror("execlp");
            _exit(1);
        } else if (pid > 0) {
            proc_table[proc_count].pid = pid;
            proc_table[proc_count].req = req;
            proc_count++;
        } else {
            perror("fork failed");
            // rollback
            sys_resources.available_ram   += req->ram_required;
            sys_resources.available_cores += req->cores_required;
            sys_resources.available_hdd   += req->hdd_required;
        }
    } else {
        enqueue(req);
    }
    sem_post(&resource_sem);
}

// ---- Init + main loop ----
void initialize_system_resources() {
    printf("Enter total RAM (MB): ");
    scanf("%d", &sys_resources.total_ram);
    sys_resources.available_ram = sys_resources.total_ram;
    printf("Enter total CPU cores: ");
    scanf("%d", &sys_resources.total_cores);
    sys_resources.available_cores = sys_resources.total_cores;
    printf("Enter total HDD space (MB): ");
    scanf("%d", &sys_resources.total_hdd);
    sys_resources.available_hdd = sys_resources.total_hdd;
}

int main() {
    display_boot_screen();
    initialize_system_resources();

    // init semaphore (1 = unlocked)
    sem_init(&resource_sem, 0, 1);
    // start monitor thread
    pthread_create(&monitor_thread, NULL, monitor_fn, NULL);

    int choice;
    int num_apps = sizeof(apps) / sizeof(apps[0]);
    while (1) {
        printf("\n=== Menu ===\n");
        for (int i = 0; i < num_apps; i++) {
            printf("%d. %s (RAM:%d, Cores:%d, HDD:%d)\n",
                   i+1, apps[i].name,
                   apps[i].ram_required,
                   apps[i].cores_required,
                   apps[i].hdd_required);
        }
        printf("9. Display status\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        } else if (choice == 9) {
            sem_wait(&resource_sem);
            display_remaining_resources();
            sem_post(&resource_sem);
        } else if (choice > 0 && choice <= num_apps) {
            launch_app(apps[choice-1].name);
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
