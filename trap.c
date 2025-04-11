#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "signal.h"
#define SYS_sigreturn  25


// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);

  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//Default hanlders
void sigterm_handler(int signum) {
    struct proc *p = myproc();
    if (p) {	
        p->killed = 1;
    }
    return;
}

int has_pending_signals(struct proc *p) {
  for (int i = 0; i < NSIGS; i++) {
    if (p->pending_signals[i]) {
      return 1; 
    }
  }
  return 0; 
}

void handle_signal(struct proc* p){
    for(int i=0; i<NSIGS; i++){
        if(p->pending_signals[i]){
        	if (p->blocked_signals & (1 << i)) {
                   continue;
                }
        	if(p->handlers[i] != SIG_DFL){
        		cprintf("in handle_signal\n");
        		memmove(p->old_tf, p->tf, sizeof(struct trapframe));
			p->tf->eip = (uint)p->handlers[i];
			p->pending_signals[i] = 0; 
			return;			
        	}
        	else{
        		switch(i){
        			case SIGTERM:
        				sigterm_handler(i);
        				break;
        			case SIGUSR1:
        				sigterm_handler(i);
        				break;
        			case SIGSEGV:
        				sigterm_handler(i);
        				break;
        			case SIGCHLD:
        				continue;
        			case SIGILL:
        				sigterm_handler(i);
        				break;
        				
        		}
        		p->pending_signals[i] = 0;
        	}        	 
        	
	   }
	}
	return;
}
void trap(struct trapframe *tf) {

  if(tf->trapno == T_PGFLT){
    cprintf("Page fault in process %s (pid %d) at address %x\n", myproc()->name, myproc()->pid, rcr2());
    myproc()->killed=1;
    return;
  }
  
  if(tf->trapno == T_ILLOP){
    myproc()->pending_signals[9]=1;
    return;
  }  
  
  if (myproc() && myproc()->state == RUNNING && has_pending_signals(myproc())) {
	handle_signal(myproc());
    }
  
  if (myproc() && myproc()->state == STOPPED) {
    return;  
  }
  if(tf->trapno == T_SYSCALL){
    
    if(myproc()->killed)
      exit();
    myproc()->tf = tf;
    syscall();
    if(myproc()->killed)
      exit();
    return;
  }
  
  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpuid() == 0){
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }
    lapiceoi();
    break;

  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;

  case T_IRQ0 + IRQ_IDE+1:
    break;

  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;

  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;

  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n", cpuid(), tf->cs, tf->eip);
    lapiceoi();
    break;

  default:
    if(myproc() == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno,
            tf->err, cpuid(), tf->eip, rcr2());
    myproc()->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running
  // until it gets to the regular system call return.)
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(myproc() && myproc()->state == RUNNING &&
     tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();
}
