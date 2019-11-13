#include <stdint.h>
#include <vector>


class Register4 {
    public:
        Register4();
        ~Register4();
        std::vector<uint32_t> regs;
};

class Register2 {
    public:
        Register2();
        ~Register2();
        std::vector<uint16_t> regs;
};