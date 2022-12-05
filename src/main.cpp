#include "board.hpp"
#include "edgetable.hpp"
#include "engines/ab_engine.hpp"
#include "engines/random_engine.hpp"
#include "heuristics.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std::chrono_literals;

int main() {
    init_edge_table();
    AB_Engine engine1;
    AB_Engine engine2;
    engine1.heuristic_function = &heuristic1;
    engine2.heuristic_function = &reward_table;

    int games = 1;
    for (int i = 0; i < games; i++) {
        Board board;
        int parity = 0;
        while (!board.is_terminal()) {
            auto start = std::chrono::steady_clock::now();
            // board.print();
            int move;
            if (parity == 0) {
                move = engine1.get_move(board, 130ms).pos;
            }
            else {
                move = engine2.get_move(board, 130ms).pos;
            }
            auto end = std::chrono::steady_clock::now();
            // std::cout << "player " << parity  << " moves " << move << std::endl;
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            // std::cout << "player " << parity  << " took " << elapsed << "ms" << std::endl;
            board.do_move(move);
            parity ^= 1;
        }
        
        // board.print();
        int score1 = board.count_player();
        int score2 = board.count_opponent();
        if (parity) std::swap(score1, score2);
        std::cout << "Engine 1: " << score1 << std::endl; 
        std::cout << "Engine 2: " << score2 << std::endl; 
    }
}
