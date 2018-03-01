#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void cut(int, bool, FILE*);
int findCutPoint(char*, int, bool);

int main(int argc, char** argv) {
  int opt;
  int width = 80;
  bool acceptPunct = false;
  while ((opt = getopt(argc, argv, "pw:")) != -1) {
    switch (opt) {
      case 'p':
        acceptPunct = true;
        break;
      case 'w':
        width = atoi(optarg);
        break;
    }
  }
  cut(width, acceptPunct, NULL);
}

void cut(int width, bool acceptPunct, FILE* file) {
  char buffer[width + 1];
  int widthCount = 0;
  int c;
  if (file == NULL) {
    //   read from stdin
    c = getchar();
  } else {
    //     //   read from file
    c = fgetc(file);
  }
  while (c != EOF) {
    if (c == '\n') {
      buffer[widthCount] = '\0';
      widthCount = 0;
    } else if (widthCount == width) {
      buffer[widthCount] = c;
      buffer[widthCount+1] = '\0';
      int cutPoint = findCutPoint(buffer, width, acceptPunct);
      printf("%.*s\n", cutPoint, buffer);
      strcpy(buffer, buffer + cutPoint);
      widthCount = width - cutPoint +1;
    } else {
      buffer[widthCount] = c;
      widthCount++;
      buffer[widthCount] = '\0';
    }
    if (file == NULL) {
      //   read from stdin
      c=getchar();
    } else {
      //   read from file
      c = fgetc(file);
    }
  }
  // printf("%d, %d, %d\n", buffer[0], buffer[1], buffer[2]);
  printf("%s\n", buffer);
}

int findCutPoint(char* str, int width, bool acceptPunct) {
  for (int i = width - 1; i <= 0; i--) {
    if (acceptPunct) {
      if (ispunct(str[i]) != 0 || str[i] == ' ') {
        return i;
      }
    } else {
      if (str[i] == ' ') {
        return i;
      }
    }
  }
  return width;
}