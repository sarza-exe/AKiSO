#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("Odebrano SIGINT, kończenie działania programu.\n");
        exit(0); // Zakończ program
    }
    printf("Odebrano sygnał: %d\n", sig);
}

int main() {
    // man sigaction
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    // Rejestracja obsługi dla wszystkich sygnałów
    for (int i = 1; i < NSIG; i++) {
        if (i == SIGKILL || i == SIGSTOP) {
            printf("Nie można przechwycić sygnału %d\n", i);
        } else {
            if (sigaction(i, &sa, NULL) == -1) {
                printf("Nie udało się zarejestrować sygnału %d\n", i);
            }
        }
    }

    printf("Program czeka na sygnały. PID: %d\n", getpid());

    //kill -11 <PID>
    while (1) {
        pause(); // Czekaj na sygnał
    }

    return 0;
}