#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void prompt();
void change_dir();
void parse_cmd();

char* user_input[50];

int main()
{
	printf("\e[1;1H\e[2J");
	printf("Welcome to \"Foo Shell\"\n __________________\n\n");
	
	while(1){
		prompt();
		if(user_input[0]){
			if(strcmp(user_input[0], "exit") == 0){ 
				break; 
			} else if(strcmp(user_input[0], "cd") == 0){
				change_dir();
			} else {
				parse_cmd();
			};
		};

	}

	printf("\nThanks for using \"Foo Shell\"\n\n");
	return 0;
}


void prompt(){
	int count = 0;
	char path[200];
	char value[200];
	getcwd(path, 200);

	printf("user@%s: ", path);
	fgets(value, 200, stdin);
	value[strlen(value) - 1] = '\0';

	char* token = strtok(value, " ");
	do{
		user_input[count] = token;
		user_input[count + 1] = '\0';
		
		count++;
		token = strtok(NULL, " ");
	} while(token != NULL);
}


void change_dir(){
	if(user_input[1] == NULL){
		printf("Missing path argument\n");
	} else {
		int status_code = chdir(user_input[1]);
		if(status_code != 0){
			printf("%s\n", strerror(errno));
		}
	};
}


void parse_cmd(){
	char* path_env = getenv("PATH");
	char** inputcpy = user_input;

	/* char prog[200]; */
	char envcpy[200];

	/* strcpy(prog, user_input[0]); */
	strcpy(envcpy, path_env);

	char* cur_exec_path = strtok(envcpy, ":");
	
	
	while(cur_exec_path != NULL){
		char file_path[200] = "";
		strcat(file_path, cur_exec_path); 
		strcat(file_path, "/"); 
		strcat(file_path,  inputcpy[0]); 
		
		if(access(file_path, F_OK) == 0){
			pid_t pid = fork();

			if(pid == 0){
				execv(file_path, inputcpy);
				exit(0);
			} else {
				wait(NULL);
			};
			
			break;
		}
		
		cur_exec_path = strtok(NULL, ":");
	} 
}

