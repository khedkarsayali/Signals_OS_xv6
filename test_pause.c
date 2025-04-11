#include "types.h"
#include "user.h"
#include "signal.h" 



void sigusr1_handler(void) {
    printf(1, " -------SIGUSR1 handler called \n");
    sigreturn();
}


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
    signal(SIGUSR1, sigusr1_handler);
    printf(1, "Child %d: I'm going to pause\n", getpid());
     // Will remain paused until a signal is received
    for(int i=0;i<1000;i++){
    	if(i==10){
    	    pause();
    	}
    	printf(1,"child is working %d\n",i);
    }
    printf(1, "Child %d: I've been resumed!\n", getpid());
    exit();
  } else {
    // Parent process
    printf(1, "Parent: Child PID is %d\n", pid);
    sleep(100); // Give child time to pause
    printf(1, "Parent: Sending sigusr1 to resume child\n");
    kill2(pid, SIGUSR1);
    wait();
    printf(1, "Parent: Child has terminated\n");
  }
  
  exit();
}
