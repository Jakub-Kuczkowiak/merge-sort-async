#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctime>
#include <thread>
#include <cstring>

const int n = 10000000; // The size of array to generate
const int m = 100000000; // max number to be generated
int* T;

typedef struct
{
    int* T;
    unsigned l;
    unsigned r;
    unsigned threadsAvailable;
} params_t;

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

void mergesort(int* T, unsigned l, unsigned r);
void* mergesort_async(void* args);

int main()
{
    srand(time(NULL));

    T = new int[n];
    generateRandomArray(T, n, m);

    int* T1 = new int[n];
    int* T2 = new int[n];
    memcpy(T1, T, sizeof(unsigned) * n);
    memcpy(T2, T, sizeof(unsigned) * n);

    unsigned int nthreads = std::thread::hardware_concurrency();

    params_t* args = new params_t[sizeof(params_t)];
    args->T = T1;
    args->l = 0;
    args->r = n - 1;
    args->threadsAvailable = 16;//nthreads - 1;

    clock_t begin = clock();

    pthread_t thread;
    mergesort_async((void*)args);

    clock_t end = clock();

    double elapsedTime = double(end - begin);

    clock_t begin2 = clock();
    mergesort(T2, 0, n - 1);
    clock_t end2 = clock();

    double elapsedTime2 = double(end2 - begin2);

    //printArray(T1, n);
    printf("\nTime needed to sort using mergsort_async: %f\n", elapsedTime);
    printf("Time needed to sort using mergesort: %f\n", elapsedTime2);

    return 0;
}

// https://www.geeksforgeeks.org/merge-sort/
void merge(int* T, unsigned l, unsigned q, unsigned r)
{
    int i, j, k;
    int n1 = q - l + 1;
    int n2 =  r - q;
 
    /* create temp arrays */
    int* L = new int[n1];
    int* R = new int[n2];
    //int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = T[l + i];
    for (j = 0; j < n2; j++)
        R[j] = T[q + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            T[k] = L[i];
            i++;
        }
        else
        {
            T[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        T[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        T[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergesort(int* T, unsigned l, unsigned r)
{
    if (l < r)
    {
        // This is the same as (l+r)/2 but avoids overflow for big data
        int q = l + (r - l) / 2; 
        mergesort(T, l, q);
        mergesort(T, q + 1, r);
        merge(T, l, q, r);
    }
}

void* mergesort_async(void* args)//,int* T, unsigned l, unsigned r)
{
    params_t _args = *(params_t *)args;
    int* T = _args.T;
    int l = _args.l;
    int r = _args.r;
    int threadsAvailable = _args.threadsAvailable;

    if (l < r)
    {
        // This is the same as (l+r)/2 but avoids overflow for big data
        int q = l + (r - l) / 2; 

        if (threadsAvailable < 1)
        {
            mergesort(T, l, q);
            mergesort(T, q + 1, r);
            return nullptr;
        }

        // Spawn in threads
        params_t* args = new params_t;
        args->T = T;
        args->l = l;
        args->r = q;
        args->threadsAvailable = threadsAvailable - 1;

        params_t* args2 = new params_t;
        args2->T = T;
        args2->l = q + 1;
        args2->r = r;
        args->threadsAvailable = threadsAvailable - 1;
        
        pthread_t thread;
        pthread_create(&thread, NULL, mergesort_async, (void*)args);
        mergesort_async((void*)args2);

        // wait for the thread to finish before merging
        pthread_join(thread, NULL);

        delete args;
        delete args2;

        merge(T, l, q, r);
    }   
}