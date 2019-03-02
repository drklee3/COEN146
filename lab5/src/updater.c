#include <stdio.h>
#include <unistd.h>
#include "logger.h"
#include "machine.h"
#include "util.h"

/**
 * @brief Updater loop (Thread #3)
 * 
 * Repeats forever after random delays, runs the algorithm to update the least
 * costs and displays the least costs at the time.
 * 
 * @param _cfg   Configuration
 * @return void* 
 */
void* run_updater(void* _cfg) {
    Config cfg = *(Config*) _cfg;

    while (!cfg.shutdown) {
        // sleep random secs 10-20
        size_t sec = rand_range(10, 20);
        log_info("Sleeping for %d seconds", sec);
        sleep(sec);

        // run algorithm to update least costs

        // output current least costs
    }

    return 0;
}