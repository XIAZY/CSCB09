#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    int opt;
    int width = 80;
    bool acceptPunct = false;
    while ((opt=getopt(argc, argv, "pw:")) != -1) {
        switch (opt) {
            case 'p':
                acceptPunct = true;
                break;
            case 'w':
                width = atoi(optarg);
                break;
        }
    }
    
}