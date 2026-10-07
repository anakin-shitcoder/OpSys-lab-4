#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

static int split_command(char *line, char *args[])
{
    int count = 0;
    char *token = strtok(line, " \t\n");

    while (token != NULL && count < MAX_ARGS - 1) {
        args[count++] = token;
        token = strtok(NULL, " \t\n");
    }

    args[count] = NULL;
    return count;
}

int main(void)
{
    char line[MAX_LINE];

    while (1) {
        char *args[MAX_ARGS];
        int argc;
        int background = 0;

        printf("mini-shell$ ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            putchar('\n');
            break;
        }

        argc = split_command(line, args);
        if (argc == 0)
            continue;

        if (strcmp(args[0], "exit") == 0)
            break;

        if (strcmp(args[argc - 1], "&") == 0) {
            background = 1;
            args[--argc] = NULL;
        }

        if (argc == 0)
            continue;

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            _exit(EXIT_FAILURE);
        }

        if (!background) {
            waitpid(pid, NULL, 0);
        } else {
            printf("Started background process %ld\n", (long)pid);
        }
    }

    return EXIT_SUCCESS;
}
