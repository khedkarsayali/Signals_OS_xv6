#include "types.h"
#include "user.h"
#include "signal.h"
int main() {
    int pid = fork();

    if (pid == 0) {
        // Child process
        int i = 0;
        while (1) {
            printf(1,"Child running... %d\n", i++);
            sleep(100);
        }
    } else {
        // Parent process
        sleep(300); // Let child run for a while
        printf(1,"Parent: Sending SIGSTOP to child\n");
        printf(1,"%d\n",pid);
        kill2(pid, SIGSTOP); // 19 is typically SIGSTOP

        sleep(300); // Child should be stopped now
        printf(1,"Parent: Sending SIGCONT to child\n");
        kill2(pid, SIGCONT); // 18 is typically SIGCONT

        sleep(300); // Child should resume
        printf(1,"Parent: Killing child\n");
        kill2(pid, SIGKILL); // SIGKILL
    }	

    return 0;
}

