#pragma once

typedef struct
{
    int* src;
    int* dest;
    unsigned l;
    unsigned r;
    unsigned threadsAvailable;
}copyparams_t;

void copy(int* src, int* dest, int l, int r);
void* copy_async(void* args);
void copytest(unsigned maxThreads);