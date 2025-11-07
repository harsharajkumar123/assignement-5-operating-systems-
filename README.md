[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/YVSzUnxj)
Assignment 5
============

In this assignment you are going to implement a simple but fully functional mutex based on the code in Chapter 28 (Figure 28.10) of the book `Operating Systems: Three Easy Pieces`. You have to implement the functions in `simple_mutex.c` to:

* lock a mutex with the `mutex_lock` function,
* unlock a mutex with a `mutex_unlock` function.

You will need the manual page for the `futex` system call, as well as the manual page for `syscall` because there is no glibc wrapper for the futex system call.

A test harness is already written for you in `main.c`. A correct execution of the test program looks something like this:

```
daniel@daniel-vb:~/work/assignment-6/build$ ./main
Starting test 1 of 10
Test 1 passed!
Starting test 2 of 10
Test 2 passed!
Starting test 3 of 10
Test 3 passed!
Starting test 4 of 10
Test 4 passed!
Starting test 5 of 10
Test 5 passed!
Starting test 6 of 10
Test 6 passed!
Starting test 7 of 10
Test 7 passed!
Starting test 8 of 10
Test 8 passed!
Starting test 9 of 10
Test 9 passed!
Starting test 10 of 10
Test 10 passed!
```

## Notes

* Implement functions in the code from Chapter 28, e.g., `atomic_bit_test_set`, with the builtin atomic functions. Refer to the following page for the list of these atomic functions. These functions are supported by GCC, a C compiler: https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
* If you use functions that start with `__atomic`, you may just use `__ATOMIC_SEQ_CST` for the memory order. This memory order guarantees all threads perform the memory operations in the same order. (This order is the program order.)
* See the manual page for `futex` (type `man futex` on a terminal). Search for a code example that shows how to use `syscall` to call the `futex` sysem call.
* To wake the waiting threads with `FUTEX_WAKE`operation, pass 1 to `syscall` to specify that at most 1 thread would be awoken.

## Tasks

* Implement the `mutex_lock` function. Use atomic builtins and bitwise operations where needed. Use the `futex` system call to put the calling process on a wait queue if necessary.
* Implement the `mutex_unlock` function.  Use atomic builtins where needed. Use the `futex` system call to wake waiting threads only if there is at least one waiter.
* Make sure to commit `grade.md`. Put your VUNet ID and number of used late days in this file. Your grade will be published in this file.

## Evaluation

Your assignment will be graded according to the following criteria:

- **75 points** for a correct implementation of `mutex_lock`.
- **20 points** for a correct implementation of `mutex_unlock`.
- **5 points** for coding style, including 1) brief comments on complex portions of code, 2) consistent formatting, and 3) consistent and proper indentation.
- **10 points** for a correct implementation of the MCS spinlock described below.

## Bonus -- MCS spinlock

Futex-based locks are useful for applications with longer critical sections, where yielding the processor to allow other threads/processes is worth the extra cost of context switching. However, for short critical sections, the cost of context switching may be higher than simply spinning and waiting. A class of algorithms called spinlocks solve these concurrency problems without invoking the operating system and simply using atomic primitives.

For bonus points, you can implement the famous MCS spinlock, named after the authors Mellor-Crummey and Scott. Pseudocode for the algorithm is given in their [seminal paper](https://web.archive.org/web/20140411142823/http://www.cise.ufl.edu/tr/DOC/REP-1992-71.pdf). There is also a nice pictorial depiction of the lock on [LWN](https://lwn.net/Articles/590243/).

This algorithm is conceptually quite simple. Each waiting thread waits by spinning on its own variable. When a lock-holder releases the lock, it toggles that local-spin variable for the next thread in a linked-list-based queue. This way, every lock and unlock operation does not cause lock state to *bounce* across all of the core-local caches in a multicore architecture. But managing this concurrent queue of waiting threads requires careful synchronization using atomic primitives.

## Disclosure of AI Usage
Create a file in the root of this directory called ai-usage.txt containing a disclosure of any Artificial Intelligence tools (e.g., ChatGPT) that you used in helping to complete this assignment. If you did not use any AI tools, simply include the sentence "I did not use AI in this assignment.".

Recall from the syllabus: "Any use of AI tools must be disclosed. This disclosure is a report that includes what AI tools you used, what it helped you with, and what it could not help you with. You will not be penalized for using AI tools, but you may be penalized for inadequate or incomplete disclosure." This implies that you must describe both how it was effective AND how it was ineffective.

# assignement-5-operating-systems-
