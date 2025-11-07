// simple_mutex.c
#include "simple_mutex.h"
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct qnode {
    _Atomic(struct qnode *) next;   // Next node in lock queue
    atomic_bool locked;             // Spin flag
} qnode_t;

static _Atomic(qnode_t *) mcs_lock = NULL;      // Global queue tail pointer
static __thread qnode_t thread_node;            // Per-thread lock node

void mutex_lock(int *mutex) {
    thread_node.next = NULL;
    atomic_store(&thread_node.locked, true);

    // Atomically join lock queue
    qnode_t *pred = atomic_exchange(&mcs_lock, &thread_node);

    if (pred) {
        atomic_store(&pred->next, &thread_node);       // Link behind predecessor
        while (atomic_load(&thread_node.locked)) {}    // Spin locally
    }
    // First node acquires lock immediately
}

void mutex_unlock(int *mutex) {
    qnode_t *succ = atomic_load(&thread_node.next);
    if (!succ) {
        qnode_t *expected = &thread_node;
        if (atomic_compare_exchange_strong(&mcs_lock, &expected, NULL)) {
            return;    // No successor; release lock
        }
        while (!(succ = atomic_load(&thread_node.next))) {}// Wait for successor
    }
    atomic_store(&succ->locked, false); // Wake next thread
}
