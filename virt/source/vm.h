#include "register.h"

enum opcode {
    MOV,
    ADD,
    SUB,
    SHL,
    SHR,
    LDR,
    JMP,
    STR,
    SWI,
    NOP,
    EXIT = 11,
};


enum mode {
    REG4,
    REG2
};

enum regType {
    R0,
    R1,
    R2,
    R3
};

class VM {
    public:
        VM();
        ~VM();
        void run(uint8_t *data, uint32_t size);
        void branch(uint8_t regType);
        void mov(uint8_t *data);
        void add(uint8_t *data);
        void sub(uint8_t *data);
        void ldr(uint8_t *data);
        void str(uint8_t *data);
        void shl(uint8_t *data);
        void shr(uint8_t *data);
        void switchMode();
        uint32_t getMemory32(uint32_t addr);
        uint16_t getMemory16(uint32_t addr);
        void setMemory32(uint32_t addr, uint32_t val);
        void setMemory16(uint32_t addr, uint16_t val);
        uint32_t decodeRegister(uint32_t val);
        uint32_t getPC();
        void setPC(uint32_t newPC);
        bool isMode4();

        uint8_t memory[0x10000];
        Register2* reg2;
        Register4* reg4;
        
        uint32_t pc;
        uint8_t op;
        enum mode mode;
};