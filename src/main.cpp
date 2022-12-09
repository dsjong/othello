#include "board.hpp"
#include "edgetable.hpp"
#include "engines/ab_engine.hpp"
#include "engines/mtdf_engine.hpp"
#include "engines/random_engine.hpp"
#include "heuristics.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std::chrono_literals;

int main() {
    init_edge_table();
    MTDF_Engine engine2;
    AB_Engine engine1;
    engine1.heuristic_function = &heuristic2;
    engine2.heuristic_function = &heuristic1;

    int games = 1;
    for (int i = 0; i < games; i++) {
        Board board;
        int parity = 0;
        while (!board.is_terminal()) {
            auto start = std::chrono::steady_clock::now();
            board.print();
            int move;
            std::string name;
            if (parity == 0) {
                move = engine1.get_move(board, 145ms).pos;
                name = engine1.name;
            }
            else {
                move = engine2.get_move(board, 150ms).pos;
                name = engine2.name;
            }
            auto end = std::chrono::steady_clock::now();
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << name << " took " << elapsed << "ms" << std::endl;
            board.do_move(move);
            parity ^= 1;
        }
        std::cout << "----------\n";
        board.print();
        int score1 = board.count_player();
        int score2 = board.count_opponent();
        if (parity) std::swap(score1, score2);
        std::cout << engine1.name << ": " << score1 << std::endl; 
        std::cout << engine2.name << ": " << score2 << std::endl; 
    }
}
