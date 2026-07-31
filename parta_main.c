#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
 
    int is_fcfs = (strcmp(argv[1], "fcfs") == 0);
    int is_rr   = (strcmp(argv[1], "rr") == 0);
 
    int first_burst;
    int quantum = 0;
 
    if (is_fcfs) {
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        first_burst = 2;
    } else if (is_rr) {
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        quantum = atoi(argv[2]);
        first_burst = 3;
    } else {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
 
    int nprocs = argc - first_burst;
 
    int* bursts = malloc(sizeof(int) * nprocs);
    if (bursts == NULL) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    for (int i = 0; i < nprocs; i++) {
        bursts[i] = atoi(argv[first_burst + i]);
    }
 
    if (is_fcfs) {
        printf("Using FCFS\n");
    } else {
        printf("Using RR(%d)\n", quantum);
    }
    printf("\n");
 
    for (int i = 0; i < nprocs; i++) {
        printf("Accepted P%d: Burst %d\n", i, bursts[i]);
    }
 
    struct pcb* procs = init_procs(bursts, nprocs);
    if (is_fcfs) {
        fcfs_run(procs, nprocs);
    } else {
        rr_run(procs, nprocs, quantum);
    }
 
    int total_wait = 0;
    for (int i = 0; i < nprocs; i++) {
        total_wait += procs[i].wait;
    }
    double avg = (double)total_wait / nprocs;
    printf("Average wait time: %.2f\n", avg);
 
    free(procs);
    free(bursts);
    return 0;
}
