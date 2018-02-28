#include <stdbool.h>
#include <stdio.h>

int main(int argc, char** argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      FILE* file = fopen(argv[i], "r");
      loopHTML(file);
      fclose(file);
    }

  } else {
    // argc == 1
    // read from stdin
    loopHTML(NULL);
  }
  return 0;
}

void loopHTML(FILE* file) {
  bool isTagged = false;
  bool isNextCharTagged = false;
  int c;
  if (file == NULL) {
    //   read from stdin
    c = getchar();
  } else {
    //   read from file
    c = fgetc(file);
  }
  while (c != EOF) {
    isTagged = isNextCharTagged;
    if (c == '<') {
      isNextCharTagged = true;
    } else if (c == '>') {
      isNextCharTagged = false;
    }

    if (isTagged == false && isNextCharTagged == false) {
      putchar(c);
    }
    if (file == NULL) {
      c = getchar();
    } else {
      c = fgetc(file);
    }
  }
}