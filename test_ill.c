#include "types.h"
#include "user.h"
#include "signal.h" // if you defined signals like SIGILL, SIGCHLD

void sigchld_handler(int signum) {
  printf(1, "Parent: Received SIGCHLD (Child terminated)\n");
  sigreturn();
}

int main() {
  signal(SIGCHLD, sigchld_handler);

  int pid = fork();
  if (pid < 0) {
    printf(1, "Fork failed\n");
    exit();
  }

  if (pid == 0) {
    // === CHILD PROCESS ===
    printf(1, "Child: About to perform illegal memory access...\n");

    // Simulate illegal instruction: call invalid function pointer
    void (*bad_func)() = (void (*)())0x123456;
    bad_func(); // Should cause SIGILL

    // Should never reach here
    printf(1, "Child: ERROR — should have been killed by SIGILL\n");
    exit();
  } else {
    // === PARENT PROCESS ===
    wait(); // Wait for child to die

    printf(1, "Parent: Child exited with status\n");
  }

  exit();
}
