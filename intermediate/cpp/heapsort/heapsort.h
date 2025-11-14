#pragma once

void print(const int* array, int size);
int parent(int index);
int left(int index);
int right(int index);
void max_heapify(int* array, int size, int index);
void build_max_heap(int* array, int size);
void heapsort(int* array, int size);
