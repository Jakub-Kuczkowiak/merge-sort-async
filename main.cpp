#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctime>
#include <cstring>
#include "main.h"
#include "mergesort.h"
#include "copy.h"

void generateRandomArray(int* T, int n, int m)
{
    for (int i = 0; i < n; i++)
        T[i] = rand() % m;
}

void printArray(int* T, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", T[i]);
        if (i != n - 1)
            printf(", ");
    }

    printf("\n");
}

int main()
{
    for (int i = 1; i < 16; i++)
        copytest(i); // i = 0 copies with default threads number for CPU

    printf("---------------------------");

    for (int i = 1; i < 16; i++)
        mergetest(i); // i = 0 sorts with default threads number for CPU

    return 0;
}