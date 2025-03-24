#define NSIGS 32

typedef void (*sighandler_t)(int);
int kill2(int pid, int signum);

#define	SIGINT	1
#define	SIGKILL	2
