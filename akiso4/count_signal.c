#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int count = 0;

void signal_handler(int sig) {
    printf("odebrano sygnał SIGUSR1 numer: %d \n", ++count);
}

int main() {
    signal(SIGUSR1, signal_handler);

    printf("Program czeka na sygnały. PID: %d\n", getpid());

    //kill -SIGUSR1 <PID>
    while (1) {
        pause();
    }

    return 0;
}