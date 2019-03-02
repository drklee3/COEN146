#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "costs.h"

/*
char* split(char* in_str, char* delimeters) {
    char splitted[10];

    // split by lines
    char* token = strtok(str, delimeters);
    while (token != NULL) {
        // append to array
        *(splitted + 1) = token;
        token = strtok(NULL, delimeters);
    }

    return splitted;
}
*/

size_t** parse_costs(FILE* fp) {
    size_t cost0;
    size_t cost1;
    size_t cost2;
    size_t cost3;
    int read;

    int row = 0;

    size_t** costs_table = (size_t **) malloc(4 * sizeof(size_t *));

    for (int i = 0; i < 4; ++i) {
        costs_table[i] = (size_t *) malloc(4 * sizeof(size_t));
    }

    while ((read = fscanf(fp, "%lu %lu %lu %lu",
        &cost0, &cost1, &cost2, &cost3)) != 0) {
            
        /*
        costs_table[row][0] = cost0;
        costs_table[row][1] = cost1;
        costs_table[row][2] = cost2;
        costs_table[row][3] = cost3;
        */
        row += 1;
    }

    fclose(fp);

    return costs_table;
}

/*
int* create_cost_table(char* table_str) {
    char lines[10] = split(table_str, "\n\r");

    for (int i = 0; i < sizeof(array); ++i) {
        char* line = lines[i];
        split(line)
    }

    int table[size];
}
*/