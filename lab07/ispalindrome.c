#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool isPal(char*);

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("need exact one argument. quitting\n");
        return 1;
    }

    char* input = argv[1];
    if (!isPal(input)) {
        return 1;
    }

    return 0;
}

bool isPal(char* input) {
    int length = strlen(input);
    // skip the last \0
    // get the mem addr of start and end char
    char* end = &input[length-1];
    char* start = &input[0];

    // while two pointers are not the same or ajacent
    while ((start != end) && (start + 1 != end)) {
        if (*start != *end) {
            // dereferenced value not the same, not pal
            return false;
        } else {
            // go to next eval
            start += 1;
            end -= 1;
        }
    }
    // eval succeed, is a pal
    return true;
}