#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    int prog_pid = atoi(argv[1]);
    printf("Sending 10 signals SIGUSR1 to PID: %d\n",prog_pid);

    for(int i = 0; i < 10; i++)
    {
        kill(prog_pid, SIGUSR1);
    }
    return 0;
}