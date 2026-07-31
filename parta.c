#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Allocates array of PCBs on heap
 * Once per burst
 * @returns pointer to array or NULL if allocation fails
 */
struct pcb* init_procs(int* bursts, int blen) {
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (procs == NULL) {
        return NULL;
    }
    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }
    return procs;
}

/**
 * prints pid, bursts left, and wait of every PCB
 */
void printall(struct pcb* procs, int plen) {
    for (int i = 0; i < plen; i++) {
        printf("P%d: burst_left=%d wait=%d\n", procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * runs process for @param amount and reduce bursts_left by @param amount
 * Wait increases by @param amount since other processes pause
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    procs[current].burst_left -= amount;
    for (int i = 0; i < plen; i++) {
        if (i != current && procs[i].burst_left > 0) {
            procs[i].wait += amount;
        }
    }
}

/**
 * first come first serve: run process in index order
 * @returns total elapsed time
 */
int fcfs_run(struct pcb* procs, int plen) {
    int time = 0;
    for (int i = 0; i < plen; i++) {
        int amount = procs[i].burst_left;
        run_proc(procs, plen, i, amount);
        time += amount;
    }
    return time;
}

/**
 * round robin helper
 * @returns index of next process with work remaining
 * @returns -1 if every process is done
 */
int rr_next(int current, struct pcb* procs, int plen) {
    for (int offset = 1; offset <= plen; offset++) {
        int idx = (current + offset) % plen;
        if (procs[idx].burst_left > 0) {
            return idx;
        }
    }
    return -1;
}
/**
 * round robin scheduler
 * uses rr_next to see what is next in the order
 * @returns total elapsed time
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int time = 0;
    int current = 0;
    while (current != -1) {
        int amount = procs[current].burst_left;
        if (amount > quantum) {
            amount = quantum;
        }
        run_proc(procs, plen, current, amount);
        time += amount;
        current = rr_next(current, procs, plen);
    }
    return time;
}

