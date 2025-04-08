#include "types.h"
#include "user.h"
#include "signal.h" 

int
main(void)
{
  int pid;
  
  printf(1, "Pause test starting\n");
  
  pid = fork();
  if(pid < 0) {
    printf(1, "Fork failed\n");
    exit();
  }
  
  if(pid == 0) {
    // Child process
    printf(1, "Child %d: I'm going to pause\n", getpid());
    pause(); // Will remain paused until a signal is received
    printf(1, "Child %d: I've been resumed!\n", getpid());
    exit();
  } else {
    // Parent process
    printf(1, "Parent: Child PID is %d\n", pid);
    sleep(100); // Give child time to pause
    printf(1, "Parent: Sending SIGCONT to resume child\n");
    kill2(pid, SIGCONT);
    wait();
    printf(1, "Parent: Child has terminated\n");
  }
  
  exit();
}
