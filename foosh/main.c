#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void prompt();
void tokenise();
void change_dir();
void parse_cmd();

char* p_input; 

int main()
{
	p_input = (char*) malloc(50*sizeof(char)); 

	printf("\e[38;5;0m\e[48;5;250m");
	printf("\e[1;1H\e[2J");
	printf(" ______________________________\n");
	printf("|                              |\n");
	printf("|          FOO SHELL           |\n");
	printf("|______________________________|\n");
	printf("|                              |\n");
	printf("| maintainer  :   Owusu K      |\n");
	printf("| organisation:   UEA          |\n");
	printf("|______________________________|\n\n");
	
	while(1){
		prompt();
		if(strcmp(p_input, "exit") == 0){ 
			break; 
		} else {
			parse_cmd();
		};

	}

	printf("\e[0m");
	printf("\e[1;1H\e[2J");
	
	free(p_input);
	
	return 0;
}


void prompt(){
	char path[200];
	char value[200];
	getcwd(path, 200);

	printf("user@%s: ", path);
	fgets(value, 200, stdin);
	value[strlen(value) - 1] = '\0';
	strcpy(p_input, value);
}


void tokenise(char* arg[200]) {
	int count = 0;
	char value[200];
	strcpy(value, p_input);
	char* token = strtok(value, " ");

	do{
		arg[count] = token;
		arg[count + 1] = '\0';
		count++;
		
		token = strtok(NULL, " ");
	} while(token != NULL);
};


void change_dir(){
	char* tokens[200];
	tokenise(tokens);

	if(tokens[1] == NULL){
		printf("Missing path argument\n");
	} else {
		int status_code = chdir(tokens[1]);
		if(status_code != 0){
			printf("%s\n", strerror(errno));
		}
	};
}

void parse_cmd(){
	char* tokens[200];
	tokenise(tokens);

	if(strcmp(tokens[0], "cd") == 0) {
		change_dir();
	} else {
	
		char envcpy[200];
		char* path_env = getenv("PATH");
		strcpy(envcpy, path_env);
	
		char* cur_exec_path = strtok(envcpy, ":");
		
		while(cur_exec_path != NULL){
			char file_path[200] = "";
			strcat(file_path, cur_exec_path); 
			strcat(file_path, "/"); 
			strcat(file_path, tokens[0]); 
			
			if(access(file_path, F_OK) == 0){
				pid_t pid = fork();

				if(pid == 0){
					execv(file_path, tokens);
					exit(0);
				} else {
					wait(NULL);
				};
				
				break;
			}
			
			cur_exec_path = strtok(NULL, ":");
		} 

	};
}
