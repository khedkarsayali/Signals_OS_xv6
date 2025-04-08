#include "types.h"
#include "user.h"
#include "signal.h"

int got_signal = 0;

void handler(int signum) {
  printf(1,"Child received signal %d in handler\n", signum);
  got_signal = 1;
  sigreturn();
}

int main() {
  int child = fork();

  if (child == 0) {
    // In child
    int pid = getpid();
    printf(1,"Child PID: %d registering handler and calling pause()\n", pid);

    // Register the handler
    signal(SIGUSR1, handler);

    pause();  // Should sleep until signal is received

    printf(1,"pause() returned in child. got_signal = %d\n", got_signal);
    exit();
  } else {
    // In parent
    sleep(5); // Let child enter pause()

    printf(1,"Parent sending SIGUSR1 to child PID %d\n", child);
    kill2(child, SIGUSR1);

    wait();  // Wait for child to exit
    printf(1,"Parent: Child exited\n");
    exit();
  }
}
