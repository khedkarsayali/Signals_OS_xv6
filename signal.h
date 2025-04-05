#define NSIGS 32

typedef void (*sighandler_t)(void);
int kill2(int pid, int signum);
int pause(void);
void sigreturn(void);

#define	SIGINT	1
#define	SIGKILL	2
#define SIGTERM 3
#define SIGCONT 4
#define SIGSTOP 5
#define SIGTEST 6

