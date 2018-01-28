#include <stdio.h>

// declearation of putsymbol func
void putSymbol(int);

int main() {
  // iterate among required numbers
  for (int number = 0; number <= 242; number++) {
    // copy the number to a new variable, ensure number won't overflow
    int number_copy = number;
    // print 1X2X3X4X5, where X stands for a operator
    for (int digit = 1; digit <= 5; digit++) {
      printf("%d", digit);
      putSymbol(number_copy % 3);
      number_copy = number_copy / 3;
    }
    // print the last 6 and newline
    printf("6\n");
  }
  return 0;
}

void putSymbol(int int_symbol) {
  switch (int_symbol) {
    // if 0, output nothing
    case 0:
      break;
    case 1:
    // if 1, output a plus
      putchar('+');
      break;
    case 2:
    // if 2, output a minus
      putchar('-');
      break;
  }
}