#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "costs.h"
#include "logger.h"

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

/**
 * @brief Updates the cost table with a new cost in both directions
 * 
 * @param tbl CostTable
 * @param msg Message from another node as [machine1, machine2, cost]
 */
void update_costs(CostTable* tbl, int* msg) {
    int x    = msg[0];
    int y    = msg[1];
    int cost = msg[2];

    pthread_mutex_lock(tbl->lock);

    tbl->table[x][y] = cost;
    tbl->table[y][x] = cost;
    
    pthread_mutex_unlock(tbl->lock);
}

/**
 * @brief Gets the index of closet machine
 * 
 * @param distances 
 * @param visited 
 * @return int 
 */
int get_closest(int* distances, int* visited) {
    int min = INT_MAX;
    int min_index;

    for (int i = 0; i < 4; ++i) {
        if (visited[i] == 0 && distances[i] <= min) {
            min = distances[i];
            min_index = i;
        }
    }

    return min_index;
}

/**
 * @brief Gets the least cost array
 * obtained with the link state algorithm using dijkstra's algorithm
 * 
 * @param tbl 
 * @param start 
 */
int* get_least_costs(CostTable* tbl, int start) {
    pthread_mutex_lock(tbl->lock);
    size_t** table = tbl->table;

    int* distances = (int*) malloc(4 * sizeof(int));
    // initialize to max int
    for (int i = 0; i < 4; ++i) {
        distances[i] = INT_MAX;
    }

    int visited[4] = {0};

    distances[start] = 0;

    for (int count = 0; count < 4; ++count) {
        int min_index = get_closest(distances, visited);
        visited[min_index] = 1;

        for (int j = 0; j < 4; ++j) {
            if (!visited[j]            // have not visited
                && table[min_index][j] // is not zero
                && distances[min_index] < 10000 // is a neighbor
                // distance to closest + given node is closer than the given node
                && distances[min_index] + table[min_index][j] < distances[j]) {
                    distances[j] = distances[min_index] + table[min_index][j];
            }
        }
    }
    pthread_mutex_unlock(tbl->lock);

    return distances;
}

/**
 * @brief Prints an array
 * 
 * @param arr  Array to print
 * @param size Size of array
 */
void print_array(int* arr, int size) {
    printf("[");

    for (int i = 0; i < size; ++i) {
        // last element
        if (i == size - 1) {
            printf("%d]\n", arr[i]);
            continue;
        }

        printf("%d ", arr[i]);
    }
}
