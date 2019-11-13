#include "Map.h"
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>


void Map::loadMapData(Player *player) {
    std::ifstream openFile("/dev/urandom");
    size_t seed = 0;

    if(openFile.is_open()) {
        openFile.read((char *)&seed, sizeof(seed));
        openFile.close();
    }
    else {
        puts("[-] Failed to open /dev/urandom ...");
        exit(0);
    }

    srand(seed);
    for(int i =0; i < 2; i++)
        loadWay(player, true);
    for(int i = 0; i < 4; i++)
        loadWay(player, false);
    loadItem();
}

int Map::getMapFile(const char *filename) {
    std::ifstream openFile(filename);
    if(openFile.is_open()) {
        std::string line;
        size_t lineNum = 0;
        while(getline(openFile, line)){
            if(lineNum == MAX_Y || line.length() != MAX_X)
                return -1;
			
            memcpy(&map[lineNum], line.data(), line.length());
            lineNum++;
		}
        openFile.close();
    }

    return 0;
}



void Map::printMap(Player *player) {
    uint8_t c = 0;
    std::pair<uint8_t, uint8_t> pos = player->getPosition();

    puts("===================== MAP ======================\n");
    for(int y = 0; y < MAX_Y; y++) {
        printf("\t");
        for(int x = 0; x < MAX_X; x++) {
            c = map[y][x];
            if(pos.first == x && pos.second == y)
                printf("o");
            else
                printf("%c", c);
        }
        printf("\n");
        
    }

    puts("================================================\n");
}

void Map::loadWay(Player *player, bool initialize) {
    int direction;
    uint8_t xPos, yPos;
    std::pair<uint8_t, uint8_t> currentPosition;

    if(initialize)
        currentPosition = player->getPosition();
    else {
        currentPosition = std::make_pair(MAX_X / 2, MAX_Y / 2);
        
    }
    
    while(1) {
        xPos = currentPosition.first;
        yPos = currentPosition.second;
        if(xPos <= MIN_X || xPos >= (MAX_X - 1)) break;
        if(yPos <= MIN_Y || yPos >= (MAX_Y - 1)) break;
        
        if(initialize)
            direction = rand() % 2;
        else
            direction = rand() % 4;

        switch(direction) {
            case 0:
                map[yPos][xPos++] = WAY;
                break;
            case 1:
                map[yPos++][xPos] = WAY;
                break;
            case 2:
                map[yPos--][xPos] = WAY;
                break;
            case 3:
                map[yPos][xPos--] = WAY;
            default: break;
        }

        currentPosition.first = xPos;
        currentPosition.second = yPos;
    }
}

void Map::loadItem() {
    std::pair<uint8_t, uint8_t> pos;
    std::vector< std::pair<uint8_t, uint8_t> > ways;
    size_t ways_size;
    
    for(int y = 0; y < MAX_Y; y++) {
        for(int x = 0; x < MAX_X; x++) {
            if(map[y][x] == WAY) {
                ways.push_back( std::make_pair(x, y));
            }
        }
    }

    ways_size = ways.size();
    if(!ways_size) {
        puts("[-] There is no way to put item.");
        exit(0);
    }

    for(int i = 0; i < ITEM_NUM_ON_MAP; i++) {
        pos = ways[rand() % ways_size];
        map[pos.second][pos.first] = ITEM;
    }

}

bool Map::isWall(uint8_t xPos, uint8_t yPos) {
    uint8_t type = map[yPos][xPos];
    if(type == WALL || type == VERTEX_WALL)
        return true;
    else
        return false;
}


bool Map::isItem(uint8_t xPos, uint8_t yPos) {
    uint8_t type = map[yPos][xPos];
    if(type != WALL && type != WAY && type != VERTEX_WALL) {
        map[yPos][xPos] = WAY;
        return true;
    }
    else
        return false;
}


void Map::up(Player *player) {
    std::pair<uint8_t, uint8_t> pos = player->getPosition();
    if(pos.second <= MIN_Y)
        return;

    pos.second--;
    if(isWall(pos.first, pos.second))
        return;

    else if(isItem(pos.first, pos.second))
        player->setItem();

    
    player->setPosition(pos);
}

void Map::down(Player *player) {
    std::pair<uint8_t, uint8_t> pos = player->getPosition();
    if(pos.second >= MAX_Y)
        return;

    pos.second++;
    if(isWall(pos.first, pos.second))
        return;

    else if(isItem(pos.first, pos.second))
        player->setItem();

    player->setPosition(pos);
}

void Map::left(Player *player) {
    std::pair<uint8_t, uint8_t> pos = player->getPosition();
    if(pos.first <= MIN_X)
        return;

    pos.first--;
    if(isWall(pos.first, pos.second))
        return;

    else if(isItem(pos.first, pos.second))
        player->setItem();

   player->setPosition(pos);
}

void Map::right(Player *player) {
    std::pair<uint8_t, uint8_t> pos = player->getPosition();
    if(pos.first >= MAX_X)
        return;

    pos.first++;
    if(isWall(pos.first, pos.second))
        return;

    else if(isItem(pos.first, pos.second))
        player->setItem();

    
    player->setPosition(pos);
}

uint8_t *Map::getMap() {
    return &map[0][0];
}