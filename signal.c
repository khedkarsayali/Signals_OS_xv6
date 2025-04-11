#include "types.h"
#include "user.h"
#define SYS_sigreturn  25
int sigreturn(void) {
  
    printf(1, "in sigreturn\n"); 
	asm volatile(
		"movl $25, %%eax\n\t"
		"int $64\n\t"
		:
		:
		: "%eax"
	    );
    return 0;
}
