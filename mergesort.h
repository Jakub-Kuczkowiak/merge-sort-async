#pragma once

typedef struct
{
    int* T;
    unsigned l;
    unsigned r;
    unsigned threadsAvailable;
} params_t;

void merge(int* T, unsigned l, unsigned q, unsigned r);
void mergesort(int* T, unsigned l, unsigned r);
void* mergesort_async(void* args);
void mergetest(unsigned maxThreads);