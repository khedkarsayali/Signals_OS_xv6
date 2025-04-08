#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

// Simple handler for SIGCHLD
void sigchld_handler(int signum) {
  printf(1, "Parent received SIGCHLD: %d\n", signum);
  sigreturn();
}

int
main(void)
{
  int pid;

 signal(SIGCHLD, sigchld_handler);
 
  pid = fork();
  if (pid < 0) {
    printf(1, "Fork failed\n");
    exit();
  }

  if (pid == 0) {
  for(int i=0;i<5;i++){
  	printf(1,"Child running\n");
  	sleep(5);
  
  }
    printf(1, "Child exiting...\n");
    
  } else {
    kill2(pid,SIGKILL);
    wait();           
    sleep(10);    
    printf(1, "Parent exiting\n");
  }

  exit();
}

