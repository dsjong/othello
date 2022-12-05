#include "edgetable.hpp"

#include <iostream>

int main() {
    init_edge_table();
    for (int i = 0; i < 59049; i++) {
        std::cout << edge_table[i] << "\n";
    }
}
