#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "costs.h"
#include "logger.h"

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

/**
 * @brief Creates a cost table object
 * 
 * @param fp          Pointer to cost file
 * @param lock        Mutex for cost table
 * @return CostTable* 
 */
CostTable* create_cost_table(FILE* fp, pthread_mutex_t* lock) {
    if (!fp) {
        log_error("Failed to read costs file");
        return 0;
    }

    if (pthread_mutex_init(lock, NULL) != 0) { 
        log_error("Failed to initialize mutex"); 
        return 0;
    }

    size_t** table = parse_costs(fp);

    // create table struct pointer
    CostTable* cost_table = malloc(sizeof(CostTable));

    cost_table->lock  = lock;
    cost_table->table = table;

    return cost_table;
}

/**
 * @brief Parses the cost file
 * 
 * @param fp        File pointer to cost file
 * @return size_t** 2d array / matrix of costs
 */
size_t** parse_costs(FILE* fp) {
    // use malloc to store the matrix on the heap instead of
    // stack as to allow for use after exiting function scope
    size_t** costs_table = (size_t**) malloc(4 * sizeof(size_t*));

    for (int i = 0; i < 4; ++i) {
        costs_table[i] = (size_t*) malloc(4 * sizeof(size_t));
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            fscanf(fp, "%zd", &costs_table[i][j]);
        }
    }

    fclose(fp);

    return costs_table;
}

/**
 * @brief Prints current neighbor costs
 * 
 * @param tbl CostTable
 */
void print_costs(CostTable* tbl) {
    pthread_mutex_lock(tbl->lock);

    // loop inside a lock
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%zd\t", tbl->table[i][j]);
        }
        printf("\n");
    }

    pthread_mutex_unlock(tbl->lock); 
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