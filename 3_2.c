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
	char *words[100], buf[1024];
	char proc[100];
	char sep[] = " ";
	while (1) {
		write(1, "Please, enter name of a new process\n", 38);
		gets(proc);
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
                //if (strcmp(words[0], "kill") == 0) {
		//	execvp("ps", );
		//	write(1, "What is a process you want kill?\n", 33);
		//	kill(getpid(), SIGTERM);
                //}
                if (strcmp(words[0], "cd") == 0) {
                        if(chdir(words[1]) == -1) {
				printf("Error cd %s\n", words[1]);
				continue;
			}
			getcwd(buf, sizeof(buf));
			printf("You are here now: %s\n", buf);
                        continue;
                }
                switch(pid = fork()) {
                case -1:
                        write(1, "Error make process\n", 19);
			perror("fork"); /* произошла ошибка */
                        exit(1); /*выход из родительского процесса*/
                case 0: //дочерний процесс, отдельный поток
                        printf("Child: PID = %d\n", getpid());
                        printf("Child: Parent PID: %d\n", getppid());
                        (void) signal(SIGINT,SIG_DFL); //установка сигналов по деф
                        execvp(words[0],words);
                default: //родительский процесс, отдельный поток
                        printf("Parent: PID = %d\n", getpid());
                        printf("Parent: Child PID: %d\n",pid);
                        printf("Parent: I'm waiting for the completion of the child process.\n");
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
