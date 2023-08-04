#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char *args[100] = {};
    char input[500] = {};
    do {
        char *word;
        char temp;
        int i = 0;
        int j = 0;
        int k = 0;
        pid_t pid;
        int fd = -1;
        char fname[100] = {};
        bool reflag = false;
        bool re2flag = false;
        printf(">>");
        while (1) {
            temp = getchar();
            if (temp == EOF || temp == '\n') {
                input[i] = '\0';  // argsreset
                break;
            }
            if (temp == '>') {
                input[i] = '\0';
                while (1) {
                    temp = getchar();
                    if (temp == ' ') {
                        continue;
                    }
                    if (temp == EOF || temp == '\n') {
                        fname[k] = '\0';
                        reflag = true;
                        // puts(fname);
                        fd = open(fname, O_CREAT | O_WRONLY, S_IRWXU);
                        if (fd == -1) {
                            perror("Error opening output file\n");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                    fname[k] = temp;
                    k += 1;
                }
                break;
            }
            if (temp == '<') {
                input[i] = '\0';
                while (1) {
                    temp = getchar();
                    if (temp == ' ') {
                        continue;
                    }
                    if (temp == EOF || temp == '\n') {
                        fname[k] = '\0';
                        re2flag = true;
                        // puts(fname);
                        fd = open(fname, O_RDONLY);
                        if (fd == -1) {
                            perror("Error opening input file\n");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                    fname[k] = temp;
                    k += 1;
                }
                break;
            }
            input[i] = temp;
            i += 1;
        }
        if (strcmp(input, "exit") == 0) {
            break;
        }
        word = strtok(input, " ");
        while (word != NULL) {
            // puts(word);//debug
            args[j] = word;
            word = strtok(NULL, " ");
            j += 1;
        }
        args[j] = NULL;

        pid = fork();
        if (pid == 0) {  // this is child
            if (reflag) {
                dup2(fd, 1);
                close(fd);
            }
            if (re2flag) {
                dup2(fd, 0);
                close(fd);
            }
            execvp(args[0], args);
            exit(0);
        } else if (pid > 0) {  // this is parent
            int status;
            waitpid(pid, &status, 0);
        }
    } while (1);

    return EXIT_SUCCESS;
}