#define _GNU_SOURCE
#include "simple_mutex.h"
#include <linux/futex.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

static int futex(int *addr, int futex_op, int val)
{
    // The syscall handles waiting and waking efficiently at the kernel level.
    return syscall(SYS_futex, addr, futex_op, val, NULL, NULL, 0);
}

// This implementation uses atomic compare-and-swap to acquire the lock.

void mutex_lock( int *mutex)
{
    int expected = 0;
     // Attempt to atomically set *mutex to 1 if it was 0 (unlocked).
    // __atomic_compare_exchange_n prevents any race conditions
    // - If *mutex == expected (0/unlocked), set *mutex = 1 and return true (lock acquired) and if it already locked it returns nothing 
    while (!__atomic_compare_exchange_n(mutex, &expected, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {

        // If we failed to acquire the lock, reset expected and wait.
        expected = 0;
        while (__atomic_load_n(mutex,__ATOMIC_SEQ_CST)!=0)
        {
            futex(mutex, FUTEX_WAIT, 1);
        }
        
    }
      
}

void mutex_unlock(int *mutex)
{
    // __atomic_store_n ensures this happens atomically so other threads don't see inconsistent state.
    __atomic_store_n(mutex, 0, __ATOMIC_SEQ_CST);
    // Wake up one waiting thread, if any.
    futex(mutex, FUTEX_WAKE, 1);
}
