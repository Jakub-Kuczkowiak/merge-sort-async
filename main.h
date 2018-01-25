#pragma once

const int n = 100000000; // The size of array to generate
const int m = 10000; // max number to be generated
const int threads = 8; // Threads on this PC
extern int* T;

void generateRandomArray(int* T, int n, int m);