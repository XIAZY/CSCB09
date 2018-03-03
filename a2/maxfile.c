#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int getMax(DIR*);

int main(int argc, char **argv) {
    if (argc == 1) {
        getMax(opendir("."));
    }
    return 0;
}

int getMax(DIR *directory) {
    struct dirent *direntp;
    while ((direntp = readdir(directory)) != NULL) {
        char *content =  direntp->d_name;
        
    }
    return 0;
}