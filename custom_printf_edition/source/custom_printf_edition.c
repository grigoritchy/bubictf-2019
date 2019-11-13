#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "printf_impl.h"


/*
The author made this challenge at version of the ubuntu 16.04.
For challenge manager or challenger, please match your ubuntu version.
*/

void myalarm(int signo) {
    exit(0);
}

int load_library(void *handle) {
    *(uint64_t *)handle = dlopen("libc.so.6", RTLD_LAZY);
    if(handle == NULL) {
        return -1;
    }

    return 0;
}

int setup_io_info(uint64_t *handle) {
    uint64_t *_IO_2_1_stdin = dlsym(handle, "_IO_2_1_stdin_");
    uint64_t *_IO_2_1_stdout = dlsym(handle, "_IO_2_1_stdout_");

    if(!_IO_2_1_stdin || !_IO_2_1_stdout) {
        return -1;
    }
    
    _IO_2_1_stdout[0] |= (uint8_t)3;
    for(int i = 1; i < 9; i++) {
        _IO_2_1_stdout[i] = (uint64_t)_IO_2_1_stdout + (uint8_t)0x83;
    }
    _IO_2_1_stdout[8] += (uint8_t)1;

    _IO_2_1_stdin[0] |= (uint8_t)3;
    _IO_2_1_stdin[0] &= ~0x200;
    for(int i = 1; i < 9; i++) {
        _IO_2_1_stdin[i] = (uint64_t)_IO_2_1_stdin + (uint8_t)0x83;
    }
    _IO_2_1_stdin[8] += (uint8_t)1;

    uint64_t *_IO_file_jumps = *(uint64_t *)(_IO_2_1_stdout + 27);
    struct vtable_io *vtable = malloc(sizeof(struct vtable_io));
    vtable->_IO_new_file_xsputn = _IO_file_jumps[7];
    
    io.state = NO_BUFFER;
    io.buf_base = io_nobuf;
    io.buf_ptr = io_nobuf;
    io.buf_end = (uint64_t)io_nobuf + (uint8_t)1;
    io.vtable = vtable;
    io.vtable->link = _IO_2_1_stdout;

    EMOJI_POUNTING = "\x9f\x98\xa1";
    EMOJI_JOY = "\x9F\x98\x82";
    EMOJI_THINKING = "\x9f\xa4\x94";
    EMOJI_ONE = "\xF0\x9F\x95\x90";
    EMOJI_TWO = "\xF0\x9F\x95\x91";
    EMOJI_THREE = "\xF0\x9F\x95\x92";
    EMOJI_FOUR = "\xF0\x9F\x95\x93";
    EMOJI_FIVE = "\xF0\x9F\x95\x94";
    EMOJI_SIX = "\xF0\x9F\x95\x95";
    EMOJI_SEVEN = "\xF0\x9F\x95\x96";
    EMOJI_EIGHT = "\xF0\x9F\x95\x97";
    EMOJI_NINE = "\xF0\x9F\x95\x98";
    EMOJI_ZERO = "\xF0\x9F\x95\x9B";

    return 0;

}

void read_string(char *buf, size_t read_size) {
    int r = 0;
    int i = 0;
    char c;

    for(i = 0; i < read_size; i++) {
        r = read(0, &c, 1);
        if(r == -1)
            break;
        if(c == 10)
            break;
        buf[i] = c;
        
    }

    buf[++i] = 0;
}

int read_int(char *buf) {
    read_string(buf, 0x10);
    return atoi(buf);
}

struct menu {
    uint64_t menu_number;
    uint64_t return_value;
    char buf[0x30];
};

struct record_info {
    char name[0x20];
    uint64_t department_length;
    char *department;
    time_t record_time;
};

struct door_record {
    uint64_t entrance_index;
    uint64_t entrance_max_index;
    uint64_t exit_index;
    uint64_t exit_max_index;
    struct record_info **entrance_record;
    struct record_info **exit_record;
};

enum {
    WORK,
    HOME,
    CHANGE_DEPARTMENT,
    SHOW_HISTORY,
    EXIT
} menu_list;


enum {
    ENTRANCE_OPTION,
    EXIT_OPTION
} option_types;


void extend_record(struct door_record *door, int option) {
    uint64_t copy_bytes = 0;
    char *tmp;
    struct record_info **new_record;

    if(option == ENTRANCE_OPTION) {
        copy_bytes = door->entrance_index * 8;
        tmp = malloc(copy_bytes);
        memcpy(tmp, door->entrance_record, copy_bytes);
        door->entrance_max_index *= 2;
        new_record = (struct record_info **)realloc(door->entrance_record, door->entrance_max_index * 8);
        memcpy(new_record, tmp, copy_bytes);
        door->entrance_record = new_record;
        free(tmp);
    }
    else if(option == EXIT_OPTION) {
        copy_bytes = door->exit_index * 8;
        tmp = malloc(copy_bytes);
        memcpy(tmp, door->exit_record, copy_bytes);
        door->exit_max_index *= 2;
        new_record = (struct record_info **)realloc(door->exit_record, door->exit_max_index * 8);
        memcpy(new_record, tmp, copy_bytes);
        door->exit_record = new_record;
        free(tmp);
    }
}

int go_to_work(struct door_record *door) {
    char department[0x1000];
    char cmd[0x30];
    time_t current_time;
    size_t department_length;
    struct record_info *record;
    int entrance_index = door->entrance_index;
    

    my_printf("Please attch your employee certificate to the machine.\n");
    my_printf("> ");
    read_string(cmd, 0x10);

    if (strncmp(cmd, "attach", 6)) return -1;

    if(entrance_index >= door->entrance_max_index) {
        extend_record(door, ENTRANCE_OPTION);
    }

    door->entrance_record[entrance_index] = malloc(sizeof(struct record_info));
    record = door->entrance_record[entrance_index];

    my_printf("Please insert your information to remain history in the machine.\n");
    my_printf("name> ");

    memset(record->name, 0, 0x20);
    read_string(record->name, 0x18);
    
    my_printf("department> ");
    memset(department, 0, 0x1000);
    read_string(department, 0xfff);
    department_length = strlen(department);
    record->department = malloc(department_length);
    memcpy(record->department, department, department_length);
    record->department_length = department_length;

    time(&current_time);
    record->record_time = current_time;

    door->entrance_index++;

    return 0;
}

int leave_office(struct door_record *door) {
    char department[0x1000];
    char cmd[0x30];
    time_t current_time;
    size_t department_length;
    struct record_info *record;
    int exit_index = door->exit_index;

    my_printf("Please attch your employee certificate to the machine.\n");
    my_printf("> ");
    read_string(cmd, 0x10);

    if (strncmp(cmd, "attach", 6)) return -1;

    if(exit_index >= door->exit_max_index) {
        extend_record(door, EXIT_OPTION);
    }

    door->exit_record[exit_index] = malloc(sizeof(struct record_info));
    record = door->exit_record[exit_index];

    my_printf("Please insert your information to remain history in the machine.\n");
    my_printf("name> ");

    memset(record->name, 0, 0x20);
    read_string(record->name, 0x18);

    my_printf("department> ");
    memset(department, 0, 0x1000);
    read_string(department, 0xfff);
    department_length = strlen(department);
    record->department = malloc(department_length);
    memcpy(record->department, department, department_length);
    record->department_length = department_length;

    time(&current_time);
    record->record_time = current_time;

    door->exit_index++;

    return 0;
}

int change_department(struct door_record *door) {
    char buf[0x20];
    char option[0x20];
    int option_type = -1;
    size_t index = 0;
    struct record_info *record;

    my_printf("Change Door History\n");
    my_printf("option> ");
    memset(option, 0, 0x20);
    read_string(option, 0x18);
    if(!strcmp(option, "entrance")) {
        option_type = ENTRANCE_OPTION;
    }
    else if(!strcmp(option, "exit")) {
        option_type = EXIT_OPTION;
    }
    else return -1;

    my_printf("index> ");
    index = read_int(buf);
    
    if(option_type == ENTRANCE_OPTION) {
        if(door->entrance_index <= 0) {
            my_printf("Doesn't exist...\n");
            return -1;
        }
        if(index >= door->entrance_index) {
            my_printf("Wrong index...\n");
            return -1;
        }

        record = door->entrance_record[index];
        my_printf("new department> ");
        read_string(record->department, record->department_length);
    }
    else if(option_type == EXIT_OPTION) {
        if(door->exit_index <= 0) {
            my_printf("Doesn't exist...\n");
            return -1;
        }
        if(index >= door->exit_index) {
            my_printf("Wrong index...\n");
            return -1;
        }

        record = door->exit_record[index];
        my_printf("new department> ");
        read_string(record->department, record->department_length);
    }

    return 0;
}

int show_history(struct door_record *door) {
    char buf[0x20];
    char name[0x20];
    char department[0x1000];
    struct record_info *record;
    my_printf("\nDoor History View\n");

    my_printf("name> ");
    memset(name, 0, 0x20);
    read_string(name, 0x18);

    my_printf("department> ");
    memset(department, 0, 0x1000);
    read_string(department, 0xfff);

    my_printf("\nemployee> \xF0\x9F\x98\x82@\xF0\x9F\x98\x82\n", name, department);
    if(door->entrance_index > 0) {
        my_printf("=== Entrance ===\n");
    }
    else {
        my_printf("There is no entrance history...\n");
    }
    for(int i = 0; i < door->entrance_index; i++) {
        record = door->entrance_record[i];
        if(!strcmp(record->name, name) && 
        !strcmp(record->department, department)) {
            itoa(i, buf);
            my_printf(buf);
            my_printf(". time> \xF0\x9F\x98\x82\n", ctime(&record->record_time));
        }
    }

    my_printf("\n\n");
    if(door->exit_index > 0) {
        my_printf("=== Leave ===\n");
    }
    else {
        my_printf("There is no leave history...\n");
    }
    for(int i = 0; i < door->exit_index; i++) {
        record = door->exit_record[i];
        if(!strcmp(record->name, name) && 
        !strcmp(record->department, department)) {
            itoa(i, buf);
            my_printf(buf);
            my_printf(". time> \xF0\x9F\x98\x82\n", ctime(&record->record_time));
        }
    }
    return 0;
}


int main() {
    void *handle = NULL;
    struct menu m;
    struct door_record *door = malloc(sizeof(struct door_record));

    signal(SIGALRM, myalarm);
    alarm(30);

    if(load_library(&handle) == -1) {
        return -1;
    }
    
    if(setup_io_info(handle) == -1) {
        return -1;
    }
    
    my_printf("Start our new door managing product with my customized library code!\n\n");

    door->entrance_index = 0;
    door->exit_index = 0;
    door->entrance_max_index = 8;
    door->exit_max_index = 8;
    door->entrance_record = (struct record_info **)malloc(door->entrance_max_index * 8);
    door->exit_record = (struct record_info **)malloc(door->exit_max_index * 8);

    while(1) {
        my_printf("[ Company Door History Manager ]\n");
        my_printf("0. Go to work.\n");
        my_printf("1. Leave the office.\n");
        my_printf("2. Change department.\n");
        my_printf("3. Show door history.\n");
        my_printf("4. Go home.\n");
        my_printf("> ");

        m.menu_number = read_int(m.buf);
        switch(m.menu_number) {
            case WORK: {
                go_to_work(door);
                break;
            }
            case HOME: {
                leave_office(door);
                break;
            }
            case CHANGE_DEPARTMENT: {
                change_department(door);
                break;
            }
            case SHOW_HISTORY: {
                show_history(door);
                break;
            }
            case EXIT: {
                if(door->entrance_index) {
                    for(int i = 0; i < door->entrance_index; i++) {
                        free(door->entrance_record[i]->department);
                        free(door->entrance_record[i]);
                    }
                }
                if(door->exit_index) {
                    for(int i = 0; i < door->exit_index; i++) {
                        free(door->exit_record[i]->department);
                        free(door->exit_record[i]);
                    }
                }

                free(door->entrance_record);
                free(door->exit_record);
                free(door);
                return 0;
            }
            default:
                break;
        }
    }

    return 0;
}