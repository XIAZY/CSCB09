#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void cut(int, bool, FILE*);
int findCutPoint(char*, int, bool);

int main(int argc, char** argv) {
  int opt;
  int width = 80;
  bool acceptPunct = false;

// get the args
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

  if (optind < argc) {
    // there're files
    do {
      FILE* file = fopen(argv[optind], "r");
      cut(width, acceptPunct, file);
    } while (++optind < argc);
  } else {
    // no file, read from stdin
    cut(width, acceptPunct, NULL);
  }
}

void cut(int width, bool acceptPunct, FILE* file) {
  char buffer[width + 1];
  int widthCount = 0;
  int c;
  if (file == NULL) {
    //   read from stdin
    c = getchar();
  } else {
    //   read from file
    c = fgetc(file);
  }
  while (c != EOF) {
    if (c == '\n') {
      // normal \n, do nothing special
      printf("%s\n", buffer);
        widthCount=0;
      buffer[widthCount] = '\0';
    } else if (widthCount == width) {
      // full
      buffer[widthCount] = (char) c;
      buffer[widthCount + 1] = '\0';
      // find where i should cut the string
      int cutPoint = findCutPoint(buffer, width, acceptPunct);
        if (cutPoint == 0) {
            cutPoint = width;
            printf("%.*s\n", width, buffer);
        } else if (cutPoint > 0) {
        // positive return value, space found.
        // delete the extra space at end
        printf("%.*s\n", cutPoint, buffer);
          cutPoint++;
      } else {
        // negative value, punc found
        // get the abs value and go
        cutPoint = -cutPoint;
        printf("%.*s\n", cutPoint, buffer);
      }
      // copy the rest string
      char tempBuffer[width];
      strcpy(tempBuffer, buffer + cutPoint);
       widthCount = width - cutPoint + 1;
      strcpy(buffer, tempBuffer);
    } else {
      // ongoing string
      buffer[widthCount] = (char) c;
      widthCount++;
      buffer[widthCount] = '\0';
    }
    if (file == NULL) {
      //   read from stdin
      c = getchar();
    } else {
      //   read from file
      c = fgetc(file);
    }
  }
  // printf("%d, %d, %d\n", buffer[0], buffer[1], buffer[2]);
  printf("%s\n", buffer);
}

int findCutPoint(char* str, int width, bool acceptPunct) {
  for (int i = width; i > 0; i--) {
    // return positive if space found, negative if punc found
    if (acceptPunct) {
      if (str[i] == ' ') {
        return i;
      } else if (ispunct(str[i]) != 0 && i == width) {
        return -(i);
      } else if (ispunct(str[i])) {
          return -(i+1);
      }
    } else {
      if (str[i] == ' ') {
        return i;
      }
    }
  }
  return 0;
}