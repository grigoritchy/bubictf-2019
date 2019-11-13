#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "vm.h"

void alarm_handler(int signo) {
    exit(0);
}

uint32_t getint() {
    char buf[16] = {0, };
    read(0, buf, 12);

    return atoi(buf);
}
int main() {
    uint8_t *code = NULL;
    uint32_t codeSize = 0;
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    alarm(30);
    signal(SIGALRM, alarm_handler);
    
    printf("size> ");
    codeSize = getint();
    code = new uint8_t[codeSize + 1];
    if(!code) {
        return 0;
    }

    bzero(code, codeSize + 1);

    printf("data> ");
    read(0, code, codeSize);

    VM* vm = new VM();
    vm->run(code, codeSize);

    delete vm;
    return 0;
}