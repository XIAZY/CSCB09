#include <stdio.h>
#include <stdlib.h>

int* getFibArray();

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("number of arguments should be strictly 1. quitting\n");
    return 0;
  } else {
    int n = atoi(argv[1]);
    if (n >= 47) {
      printf("maxinum fib index allowed is 46. quitting\n");
      return 0;
    } else {
      int *fibs;
      fibs = getFibArray();
      int fibWanted = *(fibs + n);
      printf("%d\n", fibWanted);
      return 0;
    }
  }
}

int *getFibArray() {
  static int fib[47];
  fib[0] = 0;
  fib[1] = 1;
  for (int i = 2; i <= 47; i++) {
    fib[i] = fib[i - 1] + fib[i - 2];
  }
  return fib;
}