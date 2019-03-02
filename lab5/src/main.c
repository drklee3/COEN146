#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "costs.h"
#include "logger.h"
#include "machine.h"
#include "receiver.h"
#include "updater.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: %s <machine id> <num machines> <costs file> <machines file>\n", argv[0]);
        return 1;
    }

    // parse machines file
    log_debug("fetching machines");
    FILE* machines_fp = fopen(argv[4], "r");
    Machine* machines = parse_machines(machines_fp);
    if  (!machines) {
        return 1;
    }

    print_machines(machines);
    Machine* curr_machine = get_machine(machines, argv[1]);
    if (!curr_machine) {
        log_error("Invalid machine ID");
        return 1;
    }

    // create neighbor cost table
    log_debug("creating neighbor cost table");
    FILE* costs_fp = fopen(argv[3], "r");
    pthread_mutex_t table_lock;
    CostTable* cost_table = create_cost_table(costs_fp, &table_lock);

    // check if cost table creation errored
    if (!cost_table) {
        return 1;
    }

    print_costs(cost_table);

    // create config
    log_debug("creating config");    
    Config* cfg = create_config(curr_machine);

    // reference to thread
    pthread_t thread_receiver;
    // create receiver thread #1
    log_debug("creating receiver thread");
    if (pthread_create(&thread_receiver, NULL, run_receiver, cfg)) {
        log_error("Error creating thread");
        return 1;
    }

    // create updater thread #3
    pthread_t thread_updater;
    log_debug("creating updater thread");
    if (pthread_create(&thread_updater, NULL, run_updater, cfg)) {
        log_error("Error creating thread");
        return 1;
    }
    
    if (pthread_join(thread_receiver, NULL)) {
        log_error("Failed to join receiver thread");
    }

    if (pthread_join(thread_updater, NULL)) {
        log_error("Failed to join updater thread");
    }

    return 0;
}