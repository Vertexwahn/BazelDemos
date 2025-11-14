#include "heapsort.h"

#include <algorithm>
#include <cassert>
#include <iostream>

void print(const int* array, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int parent(int index) {
    if(index < 0) {
        throw std::runtime_error("Negative index not allowed");
    }
    return (index - 1) / 2;
}

int left(int index) {
    if(index < 0) {
        throw std::runtime_error("Negative index not allowed");
    }
    return 2 * index + 1;
}

int right(int index) {
    if(index < 0) {
        throw std::runtime_error("Negative index not allowed");
    }
    return 2 * index + 2;
}

void max_heapify(int* array, int size, int index) {
    int l = left(index);
    int r = right(index);
    int index_largest = index;

    if(l < size) {
        if (array[l] > array[index]) {
            index_largest = l;
        }        
    }

    if(r < size) {
        if(array[r] > array[index_largest]) {
            index_largest = r;
        }
    }

    if(index_largest != index) {
        std::swap(array[index], array[index_largest]);
        max_heapify(array, size, index_largest);
    }
}

void build_max_heap(int* array, int size) {
    int i = parent(size - 1);

    while (i >= 0) {
        max_heapify(array, size, i);
        i--;
    }    
}

void heapsort(int* array, int size) {
    build_max_heap(array, size);
    int heapsize = size;

    while(heapsize > 1) {
        heapsize--;
        std::swap(array[heapsize], array[0]);
        max_heapify(array, heapsize, 0);
    }
}
