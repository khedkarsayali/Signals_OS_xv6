#define NSIGS 32

typedef void (*sighandler_t)(int);
int kill2(int pid, int signum);
int pause(void);
int sigreturn(void);
int sigprocmask(int how, uint mask);


#define SIG_DFL ((sighandler_t)0)
#define SIG_BLOCK   0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2

#define	SIGINT	1
#define	SIGKILL	2
#define SIGTERM 3
#define SIGCONT 4
#define SIGSTOP 5
#define SIGUSR1 6
#define SIGSEGV 7
#define SIGCHLD 8
#define SIGILL 9
#define SIGTEST 15

