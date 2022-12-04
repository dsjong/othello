#include "board.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    Board board;
    std::vector<uint64_t> moves, test_moves;

    double time = 0, test_time = 0;
    for (int seed = 0; seed < 100000; seed++) {
        board.randomize(seed);
        auto start = std::chrono::steady_clock::now();
        moves.push_back(board.get_moves());
        auto end = std::chrono::steady_clock::now();
        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    
    for (int seed = 0; seed < 100000; seed++) {
        board.randomize(seed);
        auto start = std::chrono::steady_clock::now();
        test_moves.push_back(board.test_get_moves());
        auto end = std::chrono::steady_clock::now();
        test_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    assert(moves == test_moves);
    std::cout << "Finding moves with bitboards: " << time << "us" << std::endl;
    std::cout << "Finding moves without bitboards: " << test_time << "us" << std::endl;   
}
