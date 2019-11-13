#include <stdint.h>
#include <stdlib.h>


#pragma once

#define item_t uint8_t

struct Item {
    public:
        Item() {
            byte = (item_t) rand();
        }
        item_t get() {
            return byte;
        }
    private:
        item_t byte;
};