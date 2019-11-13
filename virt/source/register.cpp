#include "register.h"


Register4::Register4() {
    regs.resize(4);
    for(uint32_t i = 0; i < 4; i++)
        regs[i] = 0;
}

Register4::~Register4() {
    regs.clear();
}


Register2::Register2() {
    regs.resize(4);
    for(uint32_t i = 0; i < 4; i++)
        regs[i] = 0;
}

Register2::~Register2() {
    regs.clear();
}