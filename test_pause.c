#include "types.h"
#include "user.h"
#include "signal.h"


void handler(void) {
    printf(1, "Signal SIGUSR1 received and handled , in handler user defeined!\n");
 
    return;
}

int main() {
    int pid = getpid();
    printf(1, "Address of handler: %p, Raw handler value: %x\n", (void*)handler, (uint)handler);
    printf(1,"Address of handler: %p\n", (void*)handler);
    printf(1, "Current PID: %d\n", pid);
   if (signal(SIGUSR1, handler) < 0) {
     printf(1, "Failed to register handler for SIGINT.\n");
       exit();
    }
 
    printf(1, "Signal handler registered. Waiting for SIGINT. Press Ctrl+C to send SIGINT.\n");

    printf(1, "Sending SIGTEST using kill2 to PID %d...\n", pid);
    if (kill2(pid, SIGUSR1) < 0) {
        printf(1, "Failed to send SIGTEST using kill2.\n");
    } else {
        printf(1, "SIGTEST sent successfully using kill2.\n");
    }
   
    sleep(5);

    printf(1, "Exiting program.\n");
    exit();
}



