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
	char path[200];
	getcwd(path, 200);

    if(argv[1]){
        check_dir(argv[1]);
    } else {
        check_dir(path);
    }
    
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
        
        printf("Permission\t\tFilename\n");
        printf("----------\t\t--------\n");

        while(dir_content != NULL) {
            char file_path[200];
            concat_path(dir_path, dir_content->d_name, file_path);
            stat(file_path, &dir_stat);
            printf("%lu\t\t\t%s\n", dir_stat.st_ino, dir_content->d_name);
        
            dir_content = readdir(dir);
        };

        closedir(dir);
    };
};



