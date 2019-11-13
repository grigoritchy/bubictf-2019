#include "vm.h"
#include <stdlib.h>
#include <string.h>

VM::VM() 
    : op(0), pc(0), mode(REG4), reg4(new Register4), reg2(new Register2) {
        bzero(memory, sizeof(memory));
}

VM::~VM() {
    delete reg4;
    delete reg2;
}

void VM::run(uint8_t *data, uint32_t size) {
    uint32_t i = 0;
    while(true) {
        i = getPC();
        op = data[i++];
        
        switch(op) {
            case MOV:
                mov(&data[i]);
                break;
            case ADD:
                add(&data[i]);
                break;
            case SUB:
                sub(&data[i]);
                break;
            case SHL:
                shl(&data[i]);
                break;
            case SHR:
                shr(&data[i]);
                break;
            case LDR:
                ldr(&data[i]);
                break;
            case JMP:
                branch(data[i]);
                break;
            case STR:
                str(&data[i]);
                break;
            case NOP:
                break;
            case EXIT:
                return;
                break;
            default: {
                exit(0);
            }
        }
    }
}


uint32_t VM::decodeRegister(uint32_t val) {
    uint32_t _val = val >> 3;
    if(_val > R3) exit(0);

    return _val;
}


void VM::mov(uint8_t *data) {
    uint8_t *p = data;
    uint32_t dest = 0, src = 0;
    uint8_t srcRaw = 0;

    if(isMode4()) {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg4->regs[dest] = src;
        }
        else {
            src = decodeRegister(*(uint16_t *)p); p+=2;
            reg4->regs[dest] = reg4->regs[src];
        }
    }
    else {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg2->regs[dest] = (uint16_t)src;
        }
        else {
            src = decodeRegister(*p); p+=2;
            reg2->regs[dest] = (uint16_t)reg2->regs[src];
        }
    }

    setPC(getPC() + (p - data) + 1);
}

void VM::add(uint8_t *data) {
    uint8_t *p = data;
    uint32_t dest = 0, src = 0;
    uint8_t srcRaw = 0;

    if(isMode4()) {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg4->regs[dest] += src;
        }
        else {
            src = decodeRegister(*(uint16_t *)p); p+=2;
            reg4->regs[dest] += reg4->regs[src];
        }
    }
    else {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg2->regs[dest] += (uint16_t)src;
        }
        else {
            src = decodeRegister(*p); p+=2;
            reg2->regs[dest] += (uint16_t)reg2->regs[src];
        }
    }

    setPC(getPC() + (p - data) + 1);
}

void VM::sub(uint8_t *data) {
    uint8_t *p = data;
    uint32_t dest = 0, src = 0;
    uint8_t srcRaw = 0;

    if(isMode4()) {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg4->regs[dest] -= src;
        }
        else {
            src = decodeRegister(*(uint16_t *)p); p+=2;
            reg4->regs[dest] -= reg4->regs[src];
        }
    }
    else {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg2->regs[dest] -= (uint16_t)src;
        }
        else {
            src = decodeRegister(*p); p+=2;
            reg2->regs[dest] -= (uint16_t)reg2->regs[src];
        }
    }

    setPC(getPC() + (p - data) + 1);
}


void VM::shl(uint8_t *data) {
    uint8_t *p = data;
    uint32_t dest = 0, src = 0;
    uint8_t srcRaw = 0;

    if(isMode4()) {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg4->regs[dest] <<= src;
        }
        else {
            src = decodeRegister(*(uint16_t *)p); p+=2;
            reg4->regs[dest] <<= reg4->regs[src];
        }
    }
    else {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg2->regs[dest] <<= (uint16_t)src;
        }
        else {
            src = decodeRegister(*p); p+=2;
            reg2->regs[dest] <<= (uint16_t)reg2->regs[src];
        }
    }

    setPC(getPC() + (p - data) + 1);
}


void VM::shr(uint8_t *data) {
    uint8_t *p = data;
    uint32_t dest = 0, src = 0;
    uint8_t srcRaw = 0;

    if(isMode4()) {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg4->regs[dest] >>= src;
        }
        else {
            src = decodeRegister(*(uint16_t *)p); p+=2;
            reg4->regs[dest] >>= reg4->regs[src];
        }
    }
    else {
        dest = *p; p++;
        srcRaw = dest & 1;
        dest = decodeRegister(dest);

        if(srcRaw) {
            src = *(uint16_t *)p; p+=2;
            reg2->regs[dest] >>= (uint16_t)src;
        }
        else {
            src = decodeRegister(*p); p+=2;
            reg2->regs[dest] >>= (uint16_t)reg2->regs[src];
        }
    }

    setPC(getPC() + (p - data) + 1);
}

void VM::ldr(uint8_t *data) {
    uint8_t *p = data;
    uint32_t dest = 0, src = 0;

    if(isMode4()) {
        dest = *p; p++;
        dest = decodeRegister(dest);
        src = decodeRegister(*(uint16_t *)p); p+=2;
        reg4->regs[dest] = getMemory32(reg4->regs[src]);
    }
    else {
        dest = *p; p++;
        dest = decodeRegister(dest);
        src = decodeRegister(*p); p+=2;
        reg2->regs[dest] = getMemory16((uint16_t)reg2->regs[src]);
    }

    setPC(getPC() + (p - data) + 1);
}


void VM::str(uint8_t *data) {
    uint8_t *p = data;
    uint32_t dest = 0, src = 0;

    if(isMode4()) {
        dest = *p; p++;
        dest = decodeRegister(dest);
        src = decodeRegister(*(uint16_t *)p); p+=2;
        setMemory32(reg4->regs[dest], reg4->regs[src]);
    }
    else {
        dest = *p; p++;
        dest = decodeRegister(dest);
        src = decodeRegister(*p); p+=2;
        setMemory16(reg2->regs[dest], (uint16_t)reg2->regs[src]);
    }

    setPC(getPC() + (p - data) + 1);
}

void VM::branch(uint8_t regType) {
    uint32_t newPC = 0;
    regType = decodeRegister(regType);

    if(isMode4()) {
        newPC = reg4->regs[regType];
    }
    else {
        newPC = (uint16_t)reg2->regs[regType];
    }

    setPC(newPC);
}


void VM::setPC(uint32_t newPC) {
    pc = newPC;
    switchMode();
}

uint32_t VM::getPC() {
    return pc;
}

bool VM::isMode4() {
    if(mode == REG4)
        return true;
    else
        return false;
}

void VM::switchMode() {
    if(isMode4()) {
        if(getPC() % 2) {
            mode = REG2;
        }
    }
    else {
        if(getPC() % 2 == 0) {
            mode = REG4;
        }
    }
}

uint32_t VM::getMemory32(uint32_t addr) {
    if(sizeof(memory) <= addr) exit(0);
    return *(uint32_t *)&memory[addr];
}

uint16_t VM::getMemory16(uint32_t addr) {
    if(sizeof(memory) <= addr) exit(0);
    return *(uint16_t *)&memory[addr];
}


void VM::setMemory32(uint32_t addr, uint32_t val) {
    if(sizeof(memory) <= addr) exit(0);
    *(uint32_t *)&memory[addr] = (uint32_t)val;
}

void VM::setMemory16(uint32_t addr, uint16_t val) {
    if(sizeof(memory) <= addr) exit(0);
    *(uint16_t *)&memory[addr] = (uint16_t)val;
}
