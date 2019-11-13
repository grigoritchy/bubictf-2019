#include "Map.h"
#include "Player.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <string>

#define MAP_FILENAME "map.txt"
#define RESET_MAX 3


enum menu {
    UP = 'u',
    DOWN = 'd',
    LEFT = 'l',
    RIGHT = 'r',
    EXIT = 'e',
    USE_ITEM = 'i',
    VIEW_ITEM = 'v',
    CHANGE_NAME = 'c',
    GET_NAME = 'n',
    RESET = 's',
};


void myalarm(int signo) {
    exit(0);
}

int main(){
    char c;
    bool done = false;
    int resetCount = 0;

    Map *map = new Map;
    Player *user = new Player;
    std::string name("guest");
    
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGALRM, myalarm);
    alarm(500);

    user->setPlayerName(name);

    map->getMapFile(MAP_FILENAME);
    map->loadMapData(user);
    while(!done) {
        map->printMap(user);
        
        puts("[ Game Menu ] ");
        puts("u : up");
        puts("d : down");
        puts("l : left");
        puts("r : right");
        puts("v : view items");
        puts("i : use item");
        puts("c : change name");
        puts("n : get name");
        puts("s : reset");
        puts("e : exit");
        printf("menu> ");

        scanf("%c", &c);
        getchar();
        switch(c) {
            case UP:
                map->up(user);
                break;
            case DOWN:
                map->down(user);
                break;
            case LEFT:
                map->left(user);
                break;
            case RIGHT:
                map->right(user);
                break;
            case VIEW_ITEM:
                user->viewItems();
                break;
            case USE_ITEM:
                user->useItem();
                break;
            case CHANGE_NAME:
                user->changePlayerName();
                break;
            case GET_NAME: {
                std::string &name = user->getPlayerName();
                printf("name: %s\n", name.c_str());
                break;
            }
            case RESET:
                if(resetCount++ >= RESET_MAX) break;
                user->setPosition(std::pair<uint8_t, uint8_t>(1, 1));
                map->getMapFile(MAP_FILENAME);
                map->loadMapData(user);
                break;

            case EXIT:
                done = true;
                break;
            default: break;
        }
    }
    
    return 0;
}