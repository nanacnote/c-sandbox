#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>


struct stat dir_stat;
struct dirent* dir_content;

void concat_path();
void check_dir();

int main(int argc, char* argv[]) {
	int i;
    char* token;
    char path[200];
	getcwd(path, 200);
    
    printf("Permission\t\tFilename\n");
    printf("----------\t\t--------\n");

    if(argc == 1){
        check_dir(path);
    } else if(argc == 2){
        token = strtok(argv[1], ":");
        
        while(token != NULL){
            check_dir(token);
            token = strtok(NULL, ":");
        };
    } else {
        for(i = 1; i < argc; i++){
            check_dir(argv[i]);  
        };
    };
    
    return 0;
};


void concat_path(char* dir_path, char* filename, char* path){
   char temp[200] = "";

   strcat(temp, dir_path);
   strcat(temp, "/");
   strcat(temp, filename);

   strcpy(path, temp);
};


void check_dir(char* dir_path){
    DIR* dir = opendir(dir_path); 

    if(dir == NULL){
        printf("%s\n", strerror(errno));
    } else {
        dir_content = readdir(dir);

        while(dir_content != NULL) {
            char file_path[200];
            concat_path(dir_path, dir_content->d_name, file_path);
            stat(file_path, &dir_stat);
            
            if(S_ISDIR(dir_stat.st_mode) == 0){
                printf((dir_stat.st_mode & S_IRUSR) ? "r" : "-");
                printf((dir_stat.st_mode & S_IWUSR) ? "w" : "-");
                printf((dir_stat.st_mode & S_IXUSR) ? "x" : "-");
                printf("\t\t\t%s\n", dir_content->d_name);
            };
        
            dir_content = readdir(dir);
        };

        closedir(dir);
    };
};



