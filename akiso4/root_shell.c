#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    //sudo chown root:root
    //sudo chmod a+s ./program

    // If the SUID bit is set and the program file is owned by root, the EUID will be 0
    if(geteuid() != 0)
    {
        fprintf(stderr, "Wrong privileges set!");
        return 1;
    }

    // Replaces current process with a new program
    execl("/bin/bash", "bash", NULL);

    // If execl fails it will return -1 and this command prints error
    perror("execl");
    return 1;
}