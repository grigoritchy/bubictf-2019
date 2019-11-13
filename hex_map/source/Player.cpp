#include "Player.h"
#include "Map.h"
#include "Item.h"
#include <iterator>
#include <stdio.h>
#include <unistd.h>

Player::Player() {
    pos = std::make_pair(1, 1);
    itemValueStorage = std::vector<item_t>(ITEM_VALUE_STORAGE_SIZE);
}

Player::~Player() {
    inventory.clear();
}

std::pair<uint8_t, uint8_t> Player::getPosition() {
    return pos;
}

void Player::setItem() {
    Item *item = new Item;
    printf("You got 0x%x byte!\n", item->get());
    inventory.push_back(item);
}


void Player::useItem() {
    signed int storageIndex = 0;
    unsigned int itemIndex = 0;
    uint8_t byte = 0;
    bool itemExist = inventory.size();
    if(!itemExist) return;

    puts("\nWhere do you want to put item?");
    printf("item> ");
    scanf("%d", &itemIndex);
    printf("storage> ");
    scanf("%d", &storageIndex);
    getchar();

    if(storageIndex < (signed int)itemValueStorage.capacity()) {
        if(itemIndex < inventory.size()) {
            byte = inventory[itemIndex]->get();
            itemValueStorage[storageIndex] = byte;
        }
        else {
            puts("[-] Wrong item");
        }
    }
    else {
        puts("[-] Wrong storage");
    }

}

bool Player::viewItems() {
    int itemIndex = 0;
    size_t inventorySize = inventory.size();
    if(!inventorySize) {
        puts("[-] Empty inventory");
        return false;
    }
    
    puts(" = Your inventory = ");
    for(std::vector<Item *>::iterator it = inventory.begin(); it != inventory.end(); it++) {
        printf("%d: 0x%x\n", itemIndex++, (*it)->get());
    }
    
    printf("\n");
    return true;
}

void Player::setPlayerName(std::string name) {
    playerName = name;   
}

void Player::changePlayerName() {
    int playerNameLength = playerName.length();
    printf("name[LIMIT:%d]> ", playerNameLength);
    read(0, (char *)playerName.data(), playerNameLength);
}

std::string& Player::getPlayerName() {
    return playerName;
}

void Player::setPosition(std::pair<uint8_t, uint8_t> pos) {
    this->pos.first = pos.first;
    this->pos.second = pos.second;
}