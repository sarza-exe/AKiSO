#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("Próba wysłania SIGKILL do procesu init (PID 1)\n");

    if (kill(1, SIGKILL) == -1) {
        printf("Nie udało się wysłać SIGKILL do init: %s\n", strerror(errno));
    } else {
        printf("SIGKILL wysłany do procesu init.\n");
    }

    return 0;
}
