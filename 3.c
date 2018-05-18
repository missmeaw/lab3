#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid, child_pid;

void main (void)
{
        int status;
	char *words[100];
	char proc[100];
	char sep[] = " ";
	while (1) {
		write(1, "Please, enter the name of the process\n", 38);
		read(0, &proc, sizeof(proc));
		int i = 0;
		int mode = 1;
		char *ptr = strtok(proc, sep);
		while (ptr != NULL) {
			words[i++] = ptr;
                        ptr = strtok(NULL, sep);
		}
                if (i > 1 && strcmp(words[i-1], "x") == 0) {
                        mode = 0;
                        words[i-1] = NULL;
                }
                else words[i] = NULL;
                if (strcmp(words[0], "break") == 0)
                        exit(0);
                if (strcmp(words[0], "cd") == 0) {
                        chdir(words[1]);
                        continue;
                }
                switch(pid = fork()) {
                case -1:
                        write(1, "Error make process\n", 19); /* произошла ошибка */
                        exit(11); /*выход из родительского процесса*/
                case 0: //дочерний процесс, отдельный поток
                        printf(" CHILD: My PID is-- %d\n", getpid());
                        printf(" CHILD: My parent PID -- %d\n", getppid());
                        printf(" CHILD: My process was changed\n");
                        (void) signal(SIGINT,SIG_DFL); //установка сигналов по деф
                        execvp(words[0],words);
                default: //родительский процесс, отдельный поток
                        printf("PARENT: My PID -- %d\n", getpid());
                        printf("PARENT: My child PID %d\n",pid);
                        printf("PARENT: I'm waiting for the completion of the child process.\n");
                        if (mode) {
                        child_pid = pid;
                        (void) signal(SIGINT,SIG_IGN); // игнор сигналов
                        child_pid = wait(&status); //ожидаем завершения доч проц
                        printf("Child has finished: PID = %d\n",child_pid);
                        if (WIFEXITED(status)) //если дочь закончилась
                        printf("Child exited with code %d\n",WIFEXITED(status));
                        else //если дочь неправильно закончилась
                                printf("Child terminated abnormally\n");
                        (void) signal(SIGINT,SIG_DFL);
                        }
                }
        }
}
void kill_p(int sig)
{
        kill(child_pid, sig);
}
