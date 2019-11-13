#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>


#define MAX_INDEX 3
#define ALLOC_MENU 1
#define WRITE_MENU 2
#define DELETE_MENU 3
#define EXIT_MENU 4


#define PINUSE_BIT 1

typedef struct s {
    char data[0x40];
} s;


typedef struct print_func {
    void (*func)(const char *);
} print_func;


typedef struct malloc_chunk {
    size_t               prev_size;
    size_t               size;
    struct malloc_chunk* fd;
    struct malloc_chunk* bk;
} malloc_chunk;

print_func *global;


s *ptr_list[MAX_INDEX];
char debug;


size_t malloc_usable_size(void* mem) {
    if (mem != 0) {
        malloc_chunk* p = ((char*)(mem) - sizeof(size_t) * 2);
        if (p->size & PINUSE_BIT)
            return p->size & ~(sizeof(size_t) - 1);
    }
    return 0;
}

void print_all(const char *prefix) {
    printf("New data: %s\n", prefix);
    puts("= Current data list =");
    for(int i = 0; i < MAX_INDEX; i++) {
        if(ptr_list[i] != NULL) {
            if(ptr_list[i]->data[0] == 0)
                printf("%d: ...\n", i);
            else
                printf("%d: %s\n", i, ptr_list[i]->data);
        }
        else
            printf("%d: ******\n", i);
    }
    printf("\n");

}


void read_size(char *buf, size_t size) {
    int r = -1;
    char c = 0;

    for(size_t i = 0; i < size; i++) {
        r = read(0, &c, 1);
        if(r <= 0) exit(0);
        if(c == '\n') return;
        buf[i] = c;
    }
}


void alloc() {
    s *ptr = NULL;
    size_t alloc_index = -1;

    for(int i = 0; i < MAX_INDEX; i++) {
        if(ptr_list[i] == NULL) {
            alloc_index = i;
            break;
        }
    }

    if(alloc_index == -1) {
        puts("[-] Allocation is full...");
        return;
    }

    ptr = malloc(sizeof(s));
    ptr_list[alloc_index] = ptr;

    puts("[+] Allocation done");

}

void write_data() {
    s *ptr = NULL;
    size_t write_index = 0;

    puts("index> ");
    scanf("%d", &write_index);

    if(write_index >= MAX_INDEX) {
        puts("[-] Index error");
        return;
    }

    ptr = ptr_list[write_index];
    if(!ptr) { 
        puts("[-] Index error");
        return;
    }

    puts("data> ");
    read_size(ptr->data, malloc_usable_size(ptr->data));
    global->func(ptr->data);
}

void delete() {
    s *ptr = NULL;
    size_t delete_index = 0;

    puts("index> ");
    scanf("%d", &delete_index);

    if(delete_index >= MAX_INDEX) {
        puts("[-] Index error");
        return;
    }

    ptr = ptr_list[delete_index];
    if(!ptr) {
        puts("[-] Index error");
        return;
    }

    free(ptr);
    ptr_list[delete_index] = NULL;

    puts("[+] Delete done");

}


int main() {
    int main_menu;
    char done = 0;

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    global = malloc(sizeof(print_func));
    if(debug)
        global->func = &emscripten_run_script;
    else
        global->func = &print_all;
        

    while(!done) {
        puts("[ Menu ] ");
        puts("1. alloc");
        puts("2. write");
        puts("3. delete");
        puts("4. exit");
        puts("> ");
        scanf("%d", &main_menu);

        switch(main_menu) {
            case ALLOC_MENU:
                alloc();
                break;
            
            case WRITE_MENU:
                write_data();
                break;
            
            case DELETE_MENU:
                delete();
                break;
            
            case EXIT_MENU:
                done = 1;
            default: break;
        }
    }

    return 0;
}