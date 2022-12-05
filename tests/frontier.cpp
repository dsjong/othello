#include "board.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

uint64_t test_get_frontier(Board &board) {
    uint64_t res = 0;
    uint64_t filled = board.player | board.opponent;
    std::vector<std::pair<int, int>> dirs = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1},
    };
    auto get_bit = [&](uint64_t board, int i, int j){
        if (i < 0 || j < 0 || i >= 8 || j >= 8) return -1;
        int pos = i * 8 + j;
        return (int) ((board & (1ull << pos)) > 0);
    };
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (get_bit(filled, i, j) == 1)
                continue;
            for (auto [di, dj] : dirs) {
                if (get_bit(board.player, i + di, j + dj) == 1) {
                    res |= 1ull << (i * 8 + j);
                    break;
                }
            }
        }
    }
    return res;
}

int main() {
    Board board;
    std::vector<uint64_t> moves, test_moves;

    double time = 0, test_time = 0;
    for (int seed = 0; seed < 100000; seed++) {
        board.randomize(seed);
        auto start = std::chrono::steady_clock::now();
        moves.push_back(board.get_frontier());
        auto end = std::chrono::steady_clock::now();
        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    
    for (int seed = 0; seed < 100000; seed++) {
        board.randomize(seed);
        auto start = std::chrono::steady_clock::now();
        test_moves.push_back(test_get_frontier(board));
        auto end = std::chrono::steady_clock::now();
        test_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    assert(moves == test_moves);
    std::cout << "Finding frontiers with bitboards: " << time << "us" << std::endl;
    std::cout << "Finding frontiers without bitboards: " << test_time << "us" << std::endl;   
}
