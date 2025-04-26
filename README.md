# AKiSO - Computer Architecture and Operating Systems

## List 1 - Installing Operating Systems
- Install Arch Linux in a virtual environment. Download the ISO image and learn the basics of the system.
- Install xv6 for x86 as a reference OS implementation. On your GNU/Linux system, clone the xv6 repository from GitHub and apply the provided xv6.patch to enable compilation with GCC ≥ 12.

## List 2 - Escape Sequences and simple bash script
- Write a C program that uses ANSI escape sequences to print "Hello, World!" in each terminal color.
- Create a shell script that renames all files in a given directory (no subdirectories) to lowercase, preserving spaces and special prefixes (e.g., leading dashes). Test your script on filenames such as " ABC DEF", "-AbC aBc", and "--ABC DEF".
- Download the `hello-os` project from GitHub and apply a patch to support C code instead of assembly. Modify `boot.c` to print "Hello World!" on separate lines using all combinations of background and foreground colors.

## List 3 - ps aux clone, Linux System Information
- Write a Bash script that lists all processes with details similar to `ps aux`. Use `/proc/PID/status` or `/proc/PID/stat` to extract fields such as ppid, pid, comm, state, tty, rss, pgid, sid, and count of open files. Format each process’s data into a neat table.
- Write a Bash script that updates every second to show system statistics from `/proc` and `/sys`: network I/O speeds (in B, KB, or MB), per-core CPU usage and current frequency, uptime in days/hours/minutes/seconds, battery percentage, load average, and memory usage. Include a simple UTF-8 bar chart to display recent network speeds.
- Write a Bash script that uses REST APIs (e.g., The Cat API and Chuck Norris API) to fetch and display an image and a random quote. Use `curl` or `wget` for requests, `jq` for JSON parsing, and tools like `img2txt` or `catimg` to show the image in the terminal.
- Write a C program for xv6 named `expr` that functions as a simple calculator. The program should accept two integers and an operator as command-line arguments and print the result of the operation.

## List 4 - Simple Shell in C that supports pipes and I/O redirection
- [Linux] Implement a **simple shell program in C** called `lsh`. It should read commands, search `PATH`, and execute them (using `execvp`), support `cd`, `exit`, background jobs (`&`), and handle zombie processes by reaping children.
- [Linux] Extend your **shell to support pipes (`|`) and I/O redirection (`<`, `>`, `2>`)**. Ensure `Ctrl+C` interrupts the running commands but not the shell itself.
- Write a C program that launches a Bash shell with root privileges without using `sudo`. After compilation, set the appropriate permissions (e.g., SUID) so any user can run it to open an administrator shell without a password.
- [Linux & xv6] Write test programs in C to answer if you can catch all signals (use `kill -l`)? Print which signals are handled, send `SIGKILL` (or another signal) to PID 1 (`init`), even as root?, Are signals queued? Send multiple `SIGUSR1` to a process and report how many were received.

## List 5 - Assembly math programs
- Write a NASM (or equivalent) x86 assembly program that reads a decimal integer as text, computes the sum of its digits (e.g., "12345" → 1+2+3+4+5 = 15), and writes the result to stdout using `read` and `write` syscalls.
- Write a NASM x86 program that stores a 3×3 matrix of integers in memory, calculates the total sum and the main diagonal sum, and prints both using syscalls. Explain in GDB how the matrix is laid out and processed.
- Write a NASM x86 program that prints a 32-bit integer in hexadecimal (like `printf("%x\n", 123456)`) using the `write` syscall. Demonstrate its execution in GDB.
- Write a 32‑bit x86 assembly program that outputs all prime numbers from 1 to 100,000 to stdout.

## List 6 - Page Tables in xv6
- In xv6, implement `vmprint` in `kernel/vm.c` to display a process’s page table contents. Call `vmprint` in `exec.c` after launching a user process and in `exit` (`proc.c`). Write a `testvm.c` program that allocates five pages with `sbrk` and observe the output for both `testvm` and `ls`.
- In xv6, add system calls to report memory usage:
  - `usedvp()` returns the number of user-space virtual pages (including stack guard page).
  - `usedpp()` counts the number of user-space physical pages by scanning the page table for valid mappings.

