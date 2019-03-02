#ifndef MACHINE_H
#define MACHINE_H

#include <pthread.h>

/**
 * @brief Machine configuration
 *  
 */
typedef struct {
    char name[50];
    char ip[50];
    int  port;
} Machine;

/**
 * @brief Configuration options
 * 
 * Stores machine info as well as configuration settings for other parts.
 * This also contains shared state variables passed between threads.
 * 
 */
typedef struct {
    Machine machine;
    int*    shutdown;
} Config;

Config* create_config(Machine* machine);
Machine* parse_machines(FILE* fp);
void print_machines(Machine* machines);
Machine* get_machine(Machine* machines, char* target);

#endif
