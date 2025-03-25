#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "signal.h"

void handler(int signum) {
    printf(1,"Signal received! Exiting pause...\n");
}

int main() {
    signal(SIGINT, handler);  // Handle Ctrl+C (SIGINT)
    printf(1,"Waiting for a signal...\n");
    pause();  // Process sleeps until a signal arrives
    printf(1,"Exiting program.\n");
    return 0;
}

