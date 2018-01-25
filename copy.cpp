#include "copy.h"
#include <cstring>
#include <pthread.h>
#include <iostream>
#include "main.h"
#include <thread>

void copy(int* src, int* dest, int l, int r)
{
    if (l == r)
    {
        dest[l] = src[l];
    }
    else if (l < r)
    {
        int q = l + (r - l) / 2;

        copy(src, dest, l, q);
        copy(src, dest, q + 1, r);
    }
}

// copies dest into src from l to r
void* copy_async(void* args)
{
    copyparams_t _args = *(copyparams_t*)args;
    int* src = _args.src;
    int* dest = _args.dest;
    int l = _args.l;
    int r = _args.r;
    int threadsAvailable = _args.threadsAvailable;

    if (l == r)
    {
        dest[l] = src[l];
    }
    else if (l < r)
    {
        int q = l + (r - l) / 2;

        if (threadsAvailable < 1)
        {
            memcpy(&dest[l], &src[l], sizeof(int) * (r - l + 1));
            return nullptr;
        }

        // Spawn in thread
        copyparams_t args;
        args.src = src;
        args.dest = dest;
        args.l = l;
        args.r = q;
        args.threadsAvailable = threadsAvailable - 1;

        copyparams_t args2;
        args2.src = src;
        args2.dest = dest;
        args2.l = q + 1;
        args2.r = r;
        args2.threadsAvailable = threadsAvailable - 1;
        
        pthread_t thread;
        pthread_create(&thread, NULL, copy_async, &args);
        copy_async(&args2);

        // wait for the thread to finish before merging
        pthread_join(thread, NULL);
    }
}

void copytest(unsigned maxThreads)
{
    int* src = new int[n];
    int* dest = new int[n];
    generateRandomArray(src, n, m);

    if (maxThreads == 0)
        maxThreads = std::thread::hardware_concurrency();

    printf("Copy test using %d threads\n", maxThreads);

    copyparams_t args;
    args.src = src;
    args.dest = dest;
    args.l = 0;
    args.r = n - 1;
    args.threadsAvailable = maxThreads - 1;

    clock_t begin = clock();
    memcpy(dest, src, sizeof(int) * n);
    clock_t end = clock();

    double elapsedTime = double(end - begin);

    generateRandomArray(src, n, m);

    clock_t begin2 = clock();
    copy_async(&args);
    clock_t end2 = clock();

    double elapsedTime2 = double(end2 - begin2);

    delete[] src;
    delete[] dest;

    printf("Time needed to copy using memcpy: %f\n", elapsedTime);
    printf("Time neeeded to copy using copy_async: %f\n\n", elapsedTime2);
}