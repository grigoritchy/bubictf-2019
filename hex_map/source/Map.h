#include "Player.h"
#include <utility>

#pragma once

#define MIN_X 0
#define MIN_Y 0
#define MAX_X 32
#define MAX_Y 32
#define ITEM_NUM_ON_MAP 1

enum mapTypes {
    VERTEX_WALL = '@',
    WALL = '#',
    ITEM = '!',
    WAY = ' '
};

class Map {
    public:
        int getMapFile(const char* filename);
        void printMap(Player *player);
        void loadMapData(Player *player);
        void loadWay(Player *player, bool initialize);
        void loadItem();
        uint8_t *getMap();
        bool isWall(uint8_t xPos, uint8_t yPos);
        bool isItem(uint8_t xPos, uint8_t yPos);
        void up(Player *player);
        void down(Player *player);
        void left(Player *player);
        void right(Player *player);

    private:
        uint8_t map[MAX_Y][MAX_X];
};