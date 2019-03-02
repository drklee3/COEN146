#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "machine.h"

/**
 * @brief Create a config object
 * 
 * @param name     Name of machine
 * @param ip       IP of machine
 * @param port     Port of machine
 * @return Config* 
 */
Config* create_config(Machine* machine) {
    // malloc to allocate on heap
    Config* cfg = malloc(sizeof(Config));

    cfg->shutdown = malloc(sizeof(int));
    cfg->shutdown = 0;

    // copy input args to config
    strncpy(cfg->machine.name, machine->name, sizeof(cfg->machine.name));
    strncpy(cfg->machine.ip,   machine->ip,   sizeof(cfg->machine.ip));
    cfg->machine.port = machine->port;

    return cfg;
}

/**
 * @brief Parses the machines file
 * 
 * @param fp        Pointer to machines file
 * @return Machine* Array of machines
 */
Machine* parse_machines(FILE* fp) {
    if (!fp) {
        log_error("Failed to read machines file");
        return 0;
    }

    Machine* machines = (Machine*) malloc(4 * sizeof(Machine));

    for (int i = 0; i < 4; ++i) {
        fscanf(fp, "%s", machines[i].name);
        fscanf(fp, "%s", machines[i].ip);
        fscanf(fp, "%d", &machines[i].port);
    }

    return machines;
}

/**
 * @brief Prints all machine information
 * 
 * @param machines 
 */
void print_machines(Machine* machines) {
    for (int i = 0; i < 4; ++i) {
        printf("%s => %s:%d\n",
            machines[i].name,
            machines[i].ip,
            machines[i].port);
    }
}

/**
 * @brief Gets machine with given name.
 * Returns 0 if there is no machine with searched name.
 * 
 * @param machines   Array of machines (fixed size of 4)
 * @param target     Name of machine to look for
 * @return Machine* 
 */
Machine* get_machine(Machine* machines, char* target) {
    for (int i = 0; i < 4; ++i) {
        if (strcmp(machines[i].name, target) == 0) {
            // matching name
            return &machines[i];
        }
    }

    // could not find matching machine
    return 0;
}

