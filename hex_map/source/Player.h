#include "Item.h"
#include <stdint.h>
#include <utility>
#include <vector>
#include <string>

#pragma once

#define ITEM_VALUE_STORAGE_SIZE 4

class Player {
    public:
        Player();
        ~Player();
        std::pair<uint8_t, uint8_t> getPosition();
        void setPlayerName(std::string name);
        std::string &getPlayerName();
        void changePlayerName();
        void setItem();
        void useItem();
        bool viewItems();
        void setPosition(std::pair<uint8_t , uint8_t> pos);

    private:
        std::pair <uint8_t, uint8_t> pos;
        std::vector<Item *> inventory;
        std::vector<item_t> itemValueStorage;
        std::string playerName;
};