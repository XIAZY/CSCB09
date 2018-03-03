#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *getMax(char *);
long long int getSize(char *);
char *concatPath(char *, char *);

int main(int argc, char **argv) {
  char *finalPath;
  if (argc == 1) {
    finalPath = getMax(".");
  } else {
    long long int finalSize = 0;
    for (int i = 1; i < argc; i++) {
      char *path = getMax(argv[i]);
      long long int size = getSize(path);
      if (size > finalSize) {
        finalSize = size;
        finalPath = path;
      }
    }
  }
  printf("%s\n", finalPath);
  return 0;
}

char *getMax(char *directory) {
  DIR *dir = opendir(directory);
  long long int finalSize = 0;
  static char finalPath[1000];
  struct dirent *direntp;
  while ((direntp = readdir(dir)) != NULL) {
    char *subname = direntp->d_name;
    char *path;
    if (strcmp(directory, ".") != 0 && strcmp(subname, ".") != 0 && strcmp(subname, "..") != 0) {
      path = concatPath(directory, subname);
    } else {
      path = subname;
    }
    if (strcmp(subname, ".") != 0 && strcmp(subname, "..") != 0) {
      struct stat pathStat;
      stat(path, &pathStat);
      if (S_ISREG(pathStat.st_mode)) {
        // printf("%s\n", path);
        long long int fileSize = pathStat.st_size;
        if (fileSize > finalSize) {
          finalSize = fileSize;
          strcpy(finalPath, path);
        }
      } else {
        char *greatestFilePath = getMax(path);
        long long int greatsetFileSize = getSize(greatestFilePath);
        if (greatsetFileSize > finalSize) {
          finalSize = greatsetFileSize;
          strcpy(finalPath, greatestFilePath);
        }
      }
    }
  }
  return finalPath;
}

long long int getSize(char *filePath) {
  struct stat pathStat;
  stat(filePath, &pathStat);
  return pathStat.st_size;
}

char *concatPath(char *parent, char *child) {
  static char newPath[1000];
  strcpy(newPath, parent);
  strcat(newPath, "/");
  strcat(newPath, child);
  return newPath;
}