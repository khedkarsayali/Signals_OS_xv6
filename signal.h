#define NSIGS 32

//typedef void (*sighandler_t)(void);
typedef void (*sighandler_t)(int);
int kill2(int pid, int signum);
int pause(void);

int sigreturn(void);


#define SIG_DFL ((sighandler_t)0)

#define	SIGINT	1
#define	SIGKILL	2
#define SIGTERM 3
#define SIGCONT 4
#define SIGSTOP 5
#define SIGUSR1 6
#define SIGTEST 7

