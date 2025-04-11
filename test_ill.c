#include "types.h"
#include "user.h"
#include "signal.h" 


int main() {

  int pid = fork();
  if (pid < 0) {
    printf(1, "Fork failed\n");
    exit();
  }

  if (pid == 0) {
    printf(1, "Child: performing  illegal memory access now\n");
        
    asm volatile(
        "ud2"  
    );

    printf(1, "Child: ERROR — should have been killed by SIGILL\n");
    exit();
  } else {
    wait(); 
    exit();
  }

  
}
