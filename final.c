#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>

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
    const char *cmd_name;    // executable path
    const char *display_name; // for UI
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

static system_resources_t sys_resources;

static app_requirements_t apps[] = {
    {"./calculator",  "Calculator",    512, 1, 100},
    {"./createFile",  "Create File",   256, 1, 50},
    {"./deleteFile",  "Delete File",   256, 1, 50},
    {"./fileInfo",    "File Info",     256, 1, 50},
    {"./guessNumber", "Guess Number",  256, 1, 50},
    {"./notePad",     "NotePad",      1024, 2, 200},
    {"./tiktak",      "Tic-Tac-Toe",   512, 1, 100},
    {"./time",        "Clock",         256, 1, 50},
    {"./typing_speed", "Typing Speed",  256, 2, 512},
    {"./memory_game", "Memory Game",   256, 1, 512},
    {"./song",        "Music Player",   512, 1, 100},
    {"./matrix",      "Matrix Multiplication", 512, 2, 100},
    {"./converterGame","Decimal Converter Game", 512, 1, 50},
    {"./fileRenamer", "File Renamer",   256, 1, 50},
    {"./encryptor","Text Encryptor", 512, 1, 100}
};

static proc_entry_t proc_table[MAX_PROCS];
static int proc_count = 0;

static queued_proc_t queue[MAX_QUEUE];
static int queue_front = 0, queue_rear = 0;

void display_remaining_resources() {
    printf("\nAvailable RAM: %d/%d MB (%.1f%%)\n",
        sys_resources.available_ram,
        sys_resources.total_ram,
        sys_resources.available_ram * 100.0 / sys_resources.total_ram);
    printf("Available CPU cores: %d/%d (%.1f%%)\n",
        sys_resources.available_cores,
        sys_resources.total_cores,
        sys_resources.available_cores * 100.0 / sys_resources.total_cores);
    printf("Available HDD: %d/%d MB (%.1f%%)\n",
        sys_resources.available_hdd,
        sys_resources.total_hdd,
        sys_resources.available_hdd * 100.0 / sys_resources.total_hdd);
}

int can_allocate(app_requirements_t *req) {
    return sys_resources.available_ram >= req->ram_required &&
           sys_resources.available_cores >= req->cores_required &&
           sys_resources.available_hdd >= req->hdd_required;
}

void allocate_resources(app_requirements_t *req) {
    sys_resources.available_ram   -= req->ram_required;
    sys_resources.available_cores -= req->cores_required;
    sys_resources.available_hdd   -= req->hdd_required;
}

void deallocate_resources(app_requirements_t *req) {
    sys_resources.available_ram   += req->ram_required;
    sys_resources.available_cores += req->cores_required;
    sys_resources.available_hdd   += req->hdd_required;
}

void enqueue(app_requirements_t *req) {
    if ((queue_rear + 1) % MAX_QUEUE == queue_front) {
        printf("Queue full, cannot enqueue process.\n");
        return;
    }
    queue[queue_rear].req = req;
    queue_rear = (queue_rear + 1) % MAX_QUEUE;
    printf("Process '%s' added to queue.\n", req->display_name);
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

void try_schedule_from_queue();

void handle_sigchld(int signo) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < proc_count; i++) {
            if (proc_table[i].pid == pid) {
                deallocate_resources(proc_table[i].req);
                memmove(&proc_table[i], &proc_table[i+1],
                        sizeof(proc_entry_t) * (proc_count - i - 1));
                proc_count--;
                printf("\nProcess %d exited, resources reclaimed.\n", pid);
                display_remaining_resources();
                try_schedule_from_queue();
                break;
            }
        }
    }
}

void try_schedule_from_queue() {
    int loop_count = (queue_rear - queue_front + MAX_QUEUE) % MAX_QUEUE;
    for (int i = 0; i < loop_count; i++) {
        app_requirements_t *req = dequeue();
        if (can_allocate(req)) {
            printf("Scheduling queued process: %s\n", req->display_name);
            allocate_resources(req);

            pid_t pid = fork();
            if (pid == 0) {
                execlp("gnome-terminal", "gnome-terminal", "--wait", "--", req->cmd_name, NULL);
                perror("execlp");
                exit(1);
            } else if (pid > 0) {
                proc_table[proc_count].pid = pid;
                proc_table[proc_count].req = req;
                proc_count++;
                display_remaining_resources();
            } else {
                perror("fork");
                deallocate_resources(req);
            }
        } else {
            enqueue(req);
            break;
        }
    }
}

void initialize_system_resources() {
    int ram_gb;
    while (1) {
        printf("Enter total RAM (GB, max 8): ");
        if (scanf("%d", &ram_gb) != 1 || ram_gb <= 0 || ram_gb > 8) {
            printf("Invalid input. Please enter a number between 1 and 8.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
    sys_resources.total_ram = ram_gb * 1024;
    sys_resources.available_ram = sys_resources.total_ram;

    while (1) {
        printf("Enter total CPU cores (max 8): ");
        if (scanf("%d", &sys_resources.total_cores) != 1 || sys_resources.total_cores <= 0 || sys_resources.total_cores > 8) {
            printf("Invalid input. Please enter a number between 1 and 8.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
    sys_resources.available_cores = sys_resources.total_cores;

    printf("Enter total HDD space (MB): ");
    scanf("%d", &sys_resources.total_hdd);
    sys_resources.available_hdd = sys_resources.total_hdd;
}


void display_boot_screen() {
    system("clear");

    printf("\n\n");
    printf("#############################################################\n");
    printf("#                                                           #\n");
    printf("#        ███╗   ██╗ ██████╗ ██╗   ██╗ █████╗                #\n");
    printf("#        ████╗  ██║██╔═══██╗██║   ██║██╔══██╗               #\n");
    printf("#        ██╔██╗ ██║██║   ██║██║   ██║███████║               #\n");
    printf("#        ██║╚██╗██║██║   ██║██║   ██║██╔══██║               #\n");
    printf("#        ██║ ╚████║╚██████╔╝╚██████╔╝██║  ██║               #\n");
    printf("#        ╚═╝  ╚═══╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝               #\n");
    printf("#                                                           #\n");
    printf("#         W E L C O M E   T O   N O V A  O S                #\n");
    printf("#                                                           #\n");
    printf("#############################################################\n");

    time_t now = time(NULL);
    char buf[64];
    strftime(buf, sizeof(buf), "%A, %d %B %Y %I:%M:%S %p", localtime(&now));
    printf("\n                 Current Time: %s\n", buf);

    sleep(4);
    system("clear");
}

void launch_app(const char *app_name) {
    app_requirements_t *app_req = NULL;
    int num_apps = sizeof(apps) / sizeof(apps[0]);
    for (int i = 0; i < num_apps; i++) {
        if (strcmp(apps[i].cmd_name, app_name) == 0) {
            app_req = &apps[i];
            break;
        }
    }
    if (!app_req) {
        printf("Application not found\n");
        return;
    }

    if (can_allocate(app_req)) {
        allocate_resources(app_req);
        display_remaining_resources();

        pid_t pid = fork();
        if (pid == 0) {
            execlp("gnome-terminal", "gnome-terminal", "--wait", "--", app_req->cmd_name, NULL);
            perror("execlp");
            exit(1);
        } else if (pid > 0) {
            proc_table[proc_count].pid = pid;
            proc_table[proc_count].req = app_req;
            proc_count++;
        } else {
            deallocate_resources(app_req);
            perror("fork failed");
        }
    } else {
        enqueue(app_req);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    display_boot_screen();
    initialize_system_resources();

    int choice;
    int num_apps = sizeof(apps) / sizeof(apps[0]);
    char input[256];  // Buffer for input string

    while (1) {
        printf("\n=== Nova OS - App Menu ===\n");
        printf("\nApplications:\n");
        for (int i = 0; i < num_apps; i++) {
            printf("%2d. %-20s (RAM: %4d MB, Cores: %d, HDD: %4d MB)\n",
                   i+1, apps[i].display_name,
                   apps[i].ram_required,
                   apps[i].cores_required,
                   apps[i].hdd_required);
        }
        printf("\nSystem Options:\n");
        printf("%2d. Display remaining system resources\n", num_apps + 1);
        printf(" 0. Exit Nova OS\n");
        printf("\nChoice: ");

        // Read input as string first
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }

        // Remove newline if present
        input[strcspn(input, "\n")] = 0;

        // Check if input is empty
        if (strlen(input) == 0) {
            printf("Please enter a valid choice.\n");
            continue;
        }

        // Check if input contains only digits
        int valid_input = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit(input[i])) {
                valid_input = 0;
                break;
            }
        }

        if (!valid_input) {
            printf("Invalid input. Please enter a number between 0 and %d.\n", num_apps + 1);
            continue;
        }

        // Convert string to number
        choice = atoi(input);

        // Validate range
        if (choice < 0 || choice > num_apps + 1) {
            printf("Invalid choice. Please enter a number between 0 and %d.\n", num_apps + 1);
            continue;
        }

        if (choice == 0) {
            printf("Shutting down Nova OS...\n");
            break;
        } else if (choice == num_apps + 1) {
            display_remaining_resources();
        } else if (choice > 0 && choice <= num_apps) {
            launch_app(apps[choice-1].cmd_name);
        }
    }

    return 0;
}