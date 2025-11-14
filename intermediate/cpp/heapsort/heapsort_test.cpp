#include "heapsort.h"

#include "gmock/gmock.h"

TEST(heapsort, left) {
    EXPECT_THROW(left(-1), std::runtime_error);

    for(int i = -100; i < 0; i++) {
        EXPECT_THROW(left(i), std::runtime_error);
    }

    EXPECT_THAT(left(0), 1);
    EXPECT_THAT(left(1), 3);
    EXPECT_THAT(left(2), 5);
    EXPECT_THAT(left(3), 7);
}

TEST(heapsort, right) {
    EXPECT_THROW(left(-1), std::runtime_error);

    for(int i = -100; i < 0; i++) {
        EXPECT_THROW(left(i), std::runtime_error);
    }

    EXPECT_THAT(right(0), 2);
    EXPECT_THAT(right(1), 4);
    EXPECT_THAT(right(2), 6);
    EXPECT_THAT(right(3), 8);
}

TEST(heapsort, parent) {
    EXPECT_THROW(parent(-1), std::runtime_error);

    for(int i = -100; i < 0; i++) {
        EXPECT_THROW(parent(i), std::runtime_error);
    }

    EXPECT_THAT(parent(0), 0);
    EXPECT_THAT(parent(1), 0);
    EXPECT_THAT(parent(2), 0);
    EXPECT_THAT(parent(3), 1);
    EXPECT_THAT(parent(4), 1);
    EXPECT_THAT(parent(5), 2);
    EXPECT_THAT(parent(6), 2);
    EXPECT_THAT(parent(7), 3);
    EXPECT_THAT(parent(8), 3);
    EXPECT_THAT(parent(9), 4);
    EXPECT_THAT(parent(10), 4);
    EXPECT_THAT(parent(11), 5);
    EXPECT_THAT(parent(12), 5);
    EXPECT_THAT(parent(13), 6);
    EXPECT_THAT(parent(14), 6);

    EXPECT_THAT(parent(1000), 499);
}

TEST(heapsort, max_heapify1) {
    int array[3] = {2, 8, 10};

    max_heapify(array, 3, 0);

    EXPECT_THAT(array[0], 10);
    EXPECT_THAT(array[1], 8);
    EXPECT_THAT(array[2], 2);
}

TEST(heapsort, max_heapify2) {
    int array[5] = {1, 2, 3, 4, 5};

    max_heapify(array, 5, 0);

    EXPECT_THAT(array[0], 3);
    EXPECT_THAT(array[1], 2);
    EXPECT_THAT(array[2], 1);
    EXPECT_THAT(array[3], 4);
    EXPECT_THAT(array[4], 5);
}

TEST(heapsort, max_heapify3) {
    int array[5] = {1, 6, 2, 3, 4};

    max_heapify(array, 5, 0);

    EXPECT_THAT(array[0], 6);
    EXPECT_THAT(array[1], 4);
    EXPECT_THAT(array[2], 2);
    EXPECT_THAT(array[3], 3);
    EXPECT_THAT(array[4], 1);
}

TEST(heapsort, max_heapify4) {
    int array[5] = {1, 6, 2, 3, 8};

    max_heapify(array, 5, 0);

    EXPECT_THAT(array[0], 6);
    EXPECT_THAT(array[1], 8);
    EXPECT_THAT(array[2], 2);
    EXPECT_THAT(array[3], 3);
    EXPECT_THAT(array[4], 1);
}

TEST(heapsort, build_max_heap) {
    int array[9] = {8, 5, 16, 4, 9, 12, 11, 3, 1};

    build_max_heap(array, 9);

    EXPECT_THAT(array[0], 16);
    EXPECT_THAT(array[1], 9);
    EXPECT_THAT(array[2], 12);
    EXPECT_THAT(array[3], 4);
    EXPECT_THAT(array[4], 5);
    EXPECT_THAT(array[5], 8);
    EXPECT_THAT(array[6], 11);
    EXPECT_THAT(array[7], 3);
    EXPECT_THAT(array[8], 1);
}

TEST(heapsort, build_max_heap2) {
    // generate random int array with 10 elements
    std::vector<int> array;
    for(int i = 0; i < 10; i++) {
        array.push_back(rand() % 100);
    }

    // Find maximum element in the array
    int max_element = *std::max_element(array.begin(), array.end());

    build_max_heap(array.data(), 10);
    EXPECT_THAT(array[0], max_element);
}

TEST(heapsort, GivenArrayWithOneElementWhenHeapsortThenExpectOneIdenticalElement) {
    int array[1] = {0};

    heapsort(array, 1);

    EXPECT_THAT(array[0], 0);
}

TEST(build_max_heap, GivenArrayWithTowSrotedElementsWhenHeapifyExpectInverseArray) {
    int array[2] = {0, 1};

    build_max_heap(array, 2);

    EXPECT_THAT(array[0], 1);
    EXPECT_THAT(array[1], 0);
}

TEST(heapsort, GivenArrayWithTowSrotedElementsWhenHeapsortExpectIdenticalArray) {
    int array[2] = {0, 1};

    heapsort(array, 2);

    EXPECT_THAT(array[0], 0);
    EXPECT_THAT(array[1], 1);
}

TEST(heapsort, heapsort) {
    int array[9] = {8, 5, 16, 4, 9, 12, 11, 3, 1};

    heapsort(array, 9);

    EXPECT_THAT(array[0], 1);
    EXPECT_THAT(array[1], 3);
    EXPECT_THAT(array[2], 4);
    EXPECT_THAT(array[3], 5);
    EXPECT_THAT(array[4], 8);
    EXPECT_THAT(array[5], 9);
    EXPECT_THAT(array[6], 11);
    EXPECT_THAT(array[7], 12);
    EXPECT_THAT(array[8], 16);
}

TEST(heapify, compare) {
    for(int i = 0; i < 1000000; i++) {
        // generate random array length
        int array_length = rand() % 100 + 1;

        // generate random content for array
        std::vector<int> array1, array2;
        for(int i = 0; i < array_length; i++) {
            int number = rand() % 100;
            array1.push_back(number);
            array2.push_back(number);
        }

        heapsort(array1.data(), array_length);
        std::sort(array2.begin(), array2.end());

        for(int i = 0; i < array_length; i++) {
            EXPECT_THAT(array1[i], array2[i]);
        }
    }
}
