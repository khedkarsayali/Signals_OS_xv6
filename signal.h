#define NSIGS 32

typedef void (*sighandler_t)(int);
int kill2(int pid, int signum);
int pause(void);

#define	SIGINT	1
#define	SIGKILL	2
#define SIGTERM 3
#define SIGCONT 4
#define SIGSTOP 5

