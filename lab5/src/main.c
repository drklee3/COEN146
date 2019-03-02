#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "machine.h"
#include "receiver.h"

int main(int argc, char *argv[]) {
    // tells other clients when to stop looping and die
    int shutdown = 0;

    // reference to thread
    pthread_t thread_receiver;

    // create neighbor cost table


    // create config
    Config cfg = {
        .machine = {
            .name = *argv[1],
            .ip   = *argv[2],
            .port = *argv[3],
        },
        .shutdown = &shutdown,
    };

    /* create a second thread which executes inc_x(&x) */
    if (pthread_create(&thread_receiver, NULL, run_receiver, &cfg)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    
    if (pthread_join(thread_receiver, NULL)) {
        printf("Failed to join thread\n");
    }

    return 0;
}