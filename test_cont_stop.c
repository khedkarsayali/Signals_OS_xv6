#include "types.h"
#include "user.h"
#include "signal.h"

void sigusr1_handler() {
    printf(1, "Custom SIGUSR1 handler executed in child!\n");

    // After this handler, execution resumes where it left off in child
    sigreturn();
}

int main() {
    int pid = fork();
    int counter=0;

  if (pid < 0) {
    printf(1,"Fork failed\n");
    exit();
  }

  if (pid == 0) {
    // Child process
    //signal(SIGUSR1, sigusr1_handler);
    //printf(1,"Child: Registered SIGUSR1 handler. Starting work...\n");

    while (counter < 10) {
      printf(1,"Child: Counter = %d\n", counter);
      counter++;
      sleep(1);
    }

    printf(1,"Child: Finished work normally\n");
    exit();

  } else {
    // Parent process
    sleep(3);  // Let child do some work first
    printf(1,"Parent: Sending SIGUSR1 to child (pid: %d)\n", pid);
    kill2(pid, SIGKILL);
    wait();
    printf(1,"Parent: Child exited after completing work\n");
  }
  printf(1,"after main\n");
  return 0;
  printf(1,"after retun0\n");
}
