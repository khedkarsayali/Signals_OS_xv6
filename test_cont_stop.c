#include "types.h"
#include "user.h"
#include "signal.h"


int main() {
    int pid = fork();
   
  if (pid == 0) {

    for(int i=0;i<1000;i++){
    	printf(1,"child is working %d\n",i);
    }
    printf(1,"Child done executing \n");
    exit();

  } else {
    sleep(3); 
    printf(1,"Parent: Sending SIGUSR1 to child (pid: %d)\n", pid);
    kill2(pid, SIGSTOP);
    sleep(100);
    printf(1,"Parent: Sending SIGCONT to child (pid: %d)\n", pid);
    kill2(pid, SIGCONT);
    wait();
    printf(1,"Parent: Child exited after completing work\n");
    exit();
  }

}



