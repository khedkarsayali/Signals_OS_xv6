#include "types.h"
#include "user.h"
#include "signal.h"
#define SYS_sigreturn  25

void sigusr1_handler() {
    printf(1, "Custom SIGUSR1 handler executed in child!\n");

    // After this handler, execution resumes where it left off in child
    return;
}

int main() {
    int pid = fork();

    if (pid == 0) {
        // Child process
        //signal(SIGUSR1, sigusr1_handler);  // Registering custom handler

        while (1) {
            printf(1, "Child running...\n");
            sleep(10);
        }
    } else {
        // Parent process

        sleep(20);
        printf(1, "Parent sending SIGUSR1 to child\n");
        kill2(pid, SIGKILL);

        
        wait();  // Wait for child to be killed
        printf(1, "Child terminated. Parent exiting.\n");
    }

    return 0;
}

