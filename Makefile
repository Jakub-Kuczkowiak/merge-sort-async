main: main.o
	g++ -pthread main.cpp mergesort.cpp copy.cpp -o main