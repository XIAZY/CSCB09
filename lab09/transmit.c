#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s pid c\n", argv[0]);
        return 1;
    }
    char* pid_c = argv[1];
    int pid = atoi(pid_c);
    
    char* bytes = argv[2];
    if (strlen(bytes) != 1) {
        fprintf(stderr, "c should be strictly one char\n");
        return 1;
    }

    int byte = bytes[0];
    return send_sig(pid, byte);
}

int send_sig(int pid, int byte) {
    int binary[8];
    int i=0;
    while (byte>0) {
        binary[i]=byte%2;
        byte=byte/2;
        i++;
    }

    for (int j=7; j>=0; j--) {
        int bit = binary[j];
        int sig;
        if (bit==0) {
            sig=SIGUSR1;
        } else {
            sig=SIGUSR2;
        }
        if (kill(pid, sig) == -1) {
            fprintf(stderr, "sending signal failed\n");
            return 1;
        }
        usleep(100000);
    }

    return 0;
}