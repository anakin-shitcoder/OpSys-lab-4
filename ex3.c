#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char *end;
    long n;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return EXIT_FAILURE;
    }

    n = strtol(argv[1], &end, 10);
    if (*end != '\0' || n < 0) {
        fprintf(stderr, "n must be a non-negative integer.\n");
        return EXIT_FAILURE;
    }

    for (long i = 0; i < n; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return EXIT_FAILURE;
        }

        printf("PID=%ld, PPID=%ld, fork #%ld\n",
               (long)getpid(), (long)getppid(), i + 1);
        fflush(stdout);

        sleep(5);
    }

    return EXIT_SUCCESS;
}
