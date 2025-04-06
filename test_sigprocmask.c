#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"


void sigusr1_handler(int sig) {
    printf(1, "[Child] SIGUSR1 handler called! Signal: %d\n", sig);
    sigreturn();
}

int main() {
    int pid = fork();

    if (pid == 0) {
        // Child process
        int child_pid = getpid();
        printf(1, "[Child] Started. PID = %d\n", child_pid);

        // Set handler for SIGUSR1
        signal(SIGUSR1, sigusr1_handler);

        // Block SIGUSR1
        sigprocmask(SIG_BLOCK, 1 << SIGUSR1);
        printf(1, "[Child] Blocked SIGUSR1\n");

        // Sleep so parent has time to send signal
        sleep(20);

        // Now unblock SIGUSR1
        printf(1, "[Child] Unblocking SIGUSR1\n");
        sigprocmask(SIG_UNBLOCK, 1 << SIGUSR1);

        // Wait to observe handler execution
        sleep(20);

        printf(1, "[Child] Exiting\n");
        exit();
    } else {
        // Parent process
        sleep(5); // Give child time to start and block signal
        printf(1, "[Parent] Sending SIGUSR1 to child (PID %d)\n", pid);
        kill2(pid, SIGUSR1);  // Use your custom `kill2()` syscall
        printf(1, "[Parent] Sent SIGUSR1 to child\n");

        wait();
        printf(1, "[Parent] Child exited. Test complete.\n");
        exit();
    }
}

