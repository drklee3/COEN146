#ifndef MACHINE_H
#define MACHINE_H

typedef struct {
    char name[50];
    char ip[50];
    int port;
} Machine;

typedef struct {
    Machine machine;
    int* shutdown;
} Config;

#endif
