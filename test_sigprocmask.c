#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"


void sigusr1_handler(void) {
    printf(1, " -------SIGUSR1 handler called \n");
    sigreturn();
}

int main() {
    int pid = fork();
    //sigprocmask(SIG_BLOCK, 1 << SIGUSR1);
    if (pid == 0) {
        int child_pid = getpid();
        printf(1, "child--- Started. PID = %d\n", child_pid);

        signal(SIGUSR1, sigusr1_handler);

        sigprocmask(SIG_BLOCK, 1 << SIGUSR1);
        printf(1, "child---- Blocked SIGUSR1\n");

        sleep(20);

        printf(1, "child---- Unblocking SIGUSR1\n");
        sigprocmask(SIG_UNBLOCK, 1 << SIGUSR1);

        sleep(20);

        printf(1, "child---- Exiting\n");
        exit();
    } else {
        sleep(5); 
        printf(1, "Parent---- Sending SIGUSR1 to child (PID %d)\n", pid);
        kill2(pid, SIGUSR1);  
        printf(1, "Parent---- Sent SIGUSR1 to child\n");

        wait();
        printf(1, "Parent---- Child exited. Test completed\n");
        exit();
    }
}

