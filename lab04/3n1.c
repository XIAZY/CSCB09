#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int n;

  if (argc != 2 || (n = atoi(argv[1])) <= 0) {
    fprintf(stderr, "usage: %s n\nn must be a positive integer\n", argv[0]);
    return (1);
  } else {
    while (n != 1) {
      // keep looping if n is not 1
      if (n % 2 == 0) {
        // n is even
        n = n / 2;
      } else {
        // n is odd
        n = n * 3 + 1;
      }
      //   print n
      printf("%d\n", n);
    }
  }
}