#include <stdio.h>

// declearation of functions
char getOpreatorChar(int);
void printExprssion(int[], int[]);
int getResult(int, int, int);

int main() {
  // iterate among all numbers
  for (int number = 0; number <= 242; number++) {
    //   array of digit, used to store "parsed" int list
    int digits[6];
    // same, but stores operators
    int operators[5];

    // copy the number to protect for loop
    int number_copy = number;
    // store the current index in the array
    int digitAlloc = 0;
    // cache numbers for concatination
    int cachedDigit = 0;
    // init calculation result to 0
    int result = 0;
    // loop through 1-6
    for (int digit = 1; digit <= 6; digit++) {
      // get operator
      int operator= number_copy % 3;
      if (operator== 0) {
        //   if operator is concat, cache new value to cachedDigit
        cachedDigit = cachedDigit * 10 + digit;
        // store value to array
        digits[digitAlloc] = cachedDigit;
        // last digit
        if (digit == 6) {
          // get previous operator and update result
          int prevOperator = operators[digitAlloc - 1];
          result = getResult(prevOperator, result, cachedDigit);
        }
      } else {
        //   next operator is not concat
        int real = cachedDigit * 10 + digit;
        if (digitAlloc > 0) {
          // not first digit, update result from calculation
          int prevOperator = operators[digitAlloc - 1];
          // get previous operator and update result
          result = getResult(prevOperator, result, real);
        } else {
          // first digit, just copy the value
          result = real;
        }
        // reset cashedDigit
        cachedDigit = 0;
        // store real value to array
        digits[digitAlloc] = real;
        // store operator to array
        operators[digitAlloc] = operator;
        // move index to next
        digitAlloc += 1;
      }
      // update number for next iter
      number_copy = number_copy / 3;
    }
    // digits or operators dont use full memory provided in the array
    if (digitAlloc < 5) {
      // add -1 to indicate ending
      operators[digitAlloc] = -1;
      digits[digitAlloc + 1] = -1;
    }
    // if result is 47, print it out
    if (result == 47) {
      printExprssion(digits, operators);
    }
  }
  return 0;
}

// function used for printing
void printExprssion(int digits[], int operators[]) {
  // print the first digit
  printf("%d", digits[0]);
  //   iterate the array
  for (int i = 1; i < 6; i++) {
    int digit = digits[i];
    // if find ending, break
    if (digit == -1) {
      break;
    }
    // otherwise find operator and print out
    int operator= operators[i - 1];
    printf("%c%d", getOpreatorChar(operator), digit);
  }
  //   print the last newline symbol
  printf("\n");
}

// function used to get calulation result from int operator
int getResult(int operator, int numberOne, int numberTwo) {
  int result;
  switch (operator) {
    case 0:
      result = numberOne * 10 + numberTwo;
      break;
    case 1:
      result = numberOne + numberTwo;
      break;
    case 2:
      result = numberOne - numberTwo;
      break;
    case -1:
      // array ends, copy the value
      result = numberTwo;
  }
  return result;
}

// function to convert int operator to plus or minus
char getOpreatorChar(int operator) {
  char result;
  switch (operator) {
    case 1:
      result = '+';
      break;
    case 2:
      result = '-';
      break;
  }
  return result;
}
