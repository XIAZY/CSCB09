#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fp;
  char c;

  if (argc != 2) {
    fprintf(stdout, "usage: %s filename\n", argv[0]);
    return 1;
  }

  if ((fp = fopen(argv[1], "r+")) == NULL) {
    perror(argv[1]);
    exit(1);
  }

  while ((c = getc(fp)) > 0) {
    putchar(c);
  }
  
  fclose(fp);

  return (1);
}