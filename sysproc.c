#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "signal.h"


int copyin(pde_t *pgdir, void *dst, void *src, uint size);


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int 
sys_kill2(void)
{
   int pid;
   int signum;
   if((argint(0, &pid) < 0) || (argint(1,&signum) <0))
    return -1;
   cprintf("Kill2 function will be called now \n");
  return kill2(pid,signum);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_pause(void)
{
  return pause();
}

int sys_signal(void) {
  cprintf("In sys_signal\n");

  int signum;
  sighandler_t handler;
  //void *u_handler;
  struct proc *p = myproc();
  
  if (argint(0, &signum) < 0)
    return -1;
    
  if(signum==SIGKILL){
  	cprintf("User cannot set custom handler for SIGKILL\n");
  	return -1;
  }

  if (argptr(1, (void*)&handler, sizeof(handler)) < 0)
    return -1;

  //handler = (sighandler_t)u_handler;
  p->handlers[signum] = (void*)handler;
  cprintf("Handler registered in sys_signal, handler(addr): %p\n", (void*)handler);
  return 0;
}

int
sys_sigreturn(void)
{
  cprintf("in sys_sigreturn :\n");
  struct proc *p = myproc();
  cprintf("Before sigreturn: tf->eip = %x tf->esp = %x\n", p->tf->eip, p->tf->esp);
  


  // Restore previous trapframe (process context before signal)
  memmove(p->tf, p->old_tf, sizeof(struct trapframe));
  cprintf("After sigreturn: tf->eip = %x tf->esp = %x\n", p->tf->eip, p->tf->esp);
  p->old_tf = 0;

  return 0;
}

int
sys_sigprocmask(void)
{
  int how;
  uint mask;
  struct proc *p = myproc();

  if (argint(0, &how) < 0 || argint(1, (int*)&mask) < 0)
    return -1;

  switch(how) {
    case SIG_BLOCK:
      p->blocked_signals |= mask;
      break;
    case SIG_UNBLOCK:
      p->blocked_signals &= ~mask;
      break;
    case SIG_SETMASK:
      p->blocked_signals = mask;
      break;
    default:
      return -1;
  }

  return 0;
}


