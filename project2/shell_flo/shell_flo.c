// usage: gcc shell_flo.c -o shell_flo && ./shell_flo
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h> /* exit */
#include <string.h> /* strcmp */
#include <fcntl.h> /* O_APPEND */
#include <errno.h> /* for pipe */

#define MAX_LINE 80  /* The maximum length command */

char *args[MAX_LINE/2 + 1]; /* command line arguments */
int i = 0;
int reInput = 0;
int reOutput = 0;

void exe();

int main(void)
{
	int should_run = 1; /* flag to determine when to exit program */
	while (should_run) {
		fflush(stdout);
		printf("flo>");
		char ch[MAX_LINE];
		int ampersand = 0;
		char end;
		int flag = 1;
		char fileName[MAX_LINE];
		do {
			scanf("%s%c",ch,&end);
			if (flag){
				if (!strcmp(ch,"exit")){
					return 0;
				}
				if (!strcmp(ch,"!!")){
					if (i == 0){
						printf("No commands in history\n");
					}
					break;
				}				
				while(i >= 0){
					free(args[i]);
					i-=1;
				}
				i = 0;
				reInput = 0;
				reOutput = 0;
				ampersand = 0;
				flag = 0;
			}
			if (!strcmp(ch,"&")) {
				ampersand = 1;
				break;
			}
			if (!strcmp(ch,"<")) {
				reInput = 1;
				scanf("%s%c",fileName,&end);
				break;
			}
			if (!strcmp(ch,">")) {
				reOutput = 1;
				scanf("%s%c",fileName,&end);
				break;
			}			
			args[i] = (char*)malloc(sizeof(char) * (strlen(ch) + 1));
			strcpy(args[i],ch);
			i+=1;
		}while(end != '\n');
			
		
		args[i] = NULL;
		
		pid_t pid;
		pid = fork();
		if (pid < 0) {
			printf("fork error!\n");
			return 0;
		}
		else if (pid == 0){ /* child */
			if (reInput){
				int i_fd = open(fileName, O_CREAT|O_APPEND|O_RDWR, 0666);
				if(i_fd < 0){
					printf("open error!\n");
					return 0;
				}
				int i_dup2_fd = dup2(i_fd, STDIN_FILENO);
				if(i_dup2_fd != STDIN_FILENO){
					printf("dup2 error!\n");
					return 0;
				}
				exe();
				close(i_fd);
				close(i_dup2_fd);
			}	
			else if (reOutput){
				int o_fd = open(fileName, O_CREAT|O_APPEND|O_RDWR, 0666);
				if(o_fd < 0){
					printf("open error!\n");
					return 0;
				}
				int o_dup2_fd = dup2(o_fd, STDOUT_FILENO);
				if(o_dup2_fd != STDOUT_FILENO){
					printf("dup2 error!\n");
					return 0;
				}
				exe();
				close(o_fd);
				close(o_dup2_fd);
			}
			else{
				exe();
			}
			exit(0);
		}
		else{ /* parent */
			if (!ampersand){
				wait(pid);
			}
		}
	}
	return 0;
}

void exe(){
	int pipe_pos = 0;
	int t = 0;
	while(t<i){
		if (!strcmp(args[t],"|")){
			pipe_pos = t;
		}
		t+=1;
	}
	if (!pipe_pos) execvp(args[0], args);
	else{
		char *tmp1[MAX_LINE/2 + 1];
		char *tmp2[MAX_LINE/2 + 1];
		t = 0;
		while(t < pipe_pos){
			tmp1[t] = (char*)malloc(sizeof(char) * (strlen(args[t]) + 1));
			strcpy(tmp1[t],args[t]);
			t+=1;
		}
		tmp1[t] = NULL;
		t = pipe_pos + 1;
		while(t < i){
			tmp2[t - pipe_pos - 1] = (char*)malloc(sizeof(char) * (strlen(args[t]) + 1));
			strcpy(tmp2[t - pipe_pos - 1],args[t]);
			t+=1;
		}
		tmp2[t - pipe_pos - 1] = NULL;
		int fd[2];
		int ret = pipe(fd);
		if (ret == -1){
			printf("pipe error!\n");
			return;
		}
		pid_t pid;
		pid = fork();
		if (pid < 0) {
			printf("fork error!\n");
			return;
		}
		else if (pid == 0){ /* child */
			close(fd[0]);
			int o_dup2_fd = dup2(fd[1],STDOUT_FILENO);
			execvp(tmp1[0], tmp1);
		}
		else{
			close(fd[1]);
			wait(pid);		
			int i_dup2_fd = dup2(fd[0],STDIN_FILENO);
			execvp(tmp2[0], tmp2); 
			fflush(stdout);
		}
		//ls -l | less
	}
}