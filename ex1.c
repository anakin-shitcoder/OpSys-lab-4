#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

static void print_process_info(const char *name, clock_t start)
{
    clock_t end = clock();
    double elapsed_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    printf("%s: PID=%ld, PPID=%ld, time=%.3f ms\n",
           name, (long)getpid(), (long)getppid(), elapsed_ms);
    fflush(stdout);
}

int main(void)
{
    pid_t first_child;
    pid_t second_child;
    clock_t parent_start;

    first_child = fork();
    if (first_child == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (first_child == 0) {
        clock_t start = clock();
        print_process_info("child 1", start);
        _exit(EXIT_SUCCESS);
    }

    parent_start = clock();

    second_child = fork();
    if (second_child == -1) {
        perror("fork");
        waitpid(first_child, NULL, 0);
        return EXIT_FAILURE;
    }

    if (second_child == 0) {
        clock_t start = clock();
        print_process_info("child 2", start);
        _exit(EXIT_SUCCESS);
    }

    waitpid(first_child, NULL, 0);
    waitpid(second_child, NULL, 0);

    print_process_info("main", parent_start);
    return EXIT_SUCCESS;
}
