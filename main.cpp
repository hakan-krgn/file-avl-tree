#include <chrono>
#include "favl_tree.h"

int main() {
    FAVLTree tree("../tree.dat");


    long now_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    for (int i = 0; i < 100000; ++i) {
        tree.insert(i, i);
    }
    long end_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();



    long now_nano2 = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    for (int i = 0; i < 100000; ++i) {
        tree.search(i).print();
    }
    long end_nano2 = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();



    long now_nano3 = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    for (int i = 0; i < 100000; ++i) {
        tree.remove(i);
    }
    long end_nano3 = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();


    printf("Average time for insert: %f\n", (end_nano - now_nano) / 100000 / 1000000.0);
    printf("Average time for search: %f\n", (end_nano2 - now_nano2) / 100000 / 1000000.0);
    printf("Average time for remove: %f\n", (end_nano3 - now_nano3) / 100000 / 1000000.0);

    return 0;
}
