#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

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


void check_dir(char* path){
    struct dirent* dir_content;
    struct stat dir_stat;
    DIR* dir = opendir(path); 

    if(dir == NULL){
        printf("%s\n", strerror(errno));
    } else {
        while((dir_content = readdir(dir)) != NULL) {
           printf("%s %lu\n", dir_content->d_name, dir_stat.st_ino);
        };

        closedir(dir);
    };
};

