#include "board.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    Board board;
    double time = 0;
    for (int seed = 0; seed < 100000; seed++) {
        board.randomize(seed);
        uint64_t moves = board.get_moves();
        if (moves == 0)
            continue;
        int pos = __builtin_ctzll(moves);
        // board.print();
        // std::cout << pos << "\n";
        auto start = std::chrono::steady_clock::now();
        Move move = board.do_move(pos);
        auto end = std::chrono::steady_clock::now();
        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        board.do_move(-1);
        // board.print();
        // std::cout << "----------\n";
        assert(move.flip != 0);
    }
    std::cout << "Doing moves without bitboards: " << time << "us" << std::endl;
}
