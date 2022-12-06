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
    AB_Engine engine1;
    MTDF_Engine engine2;
    engine1.heuristic_function = &heuristic1;
    engine2.heuristic_function = &heuristic1;

    Board board;
    for (int d = 0; d < 8; d++) {
        for (int seed = 0; seed < 10000; seed++) {
            board.randomize(seed);
            engine1.turn = board.count();
            engine2.turn = board.count();
            long long ans1 = engine1.search(board, -INF, INF, d, board.count());
            long long ans2 = engine2.search(board, -INF, INF, d, board.count());
            std::cout << d << " " << seed << " " << ans1 << " " << ans2 << "\n";
            assert(ans1 == ans2);
        }
    }
    return 0;

    int games = 1;
    for (int i = 0; i < games; i++) {
        Board board;
        int parity = 0;
        while (!board.is_terminal()) {
            auto start = std::chrono::steady_clock::now();
            board.print();
            int move;
            if (parity == 0) {
                move = engine1.get_move(board, 145ms).pos;
            }
            else {
                move = engine2.get_move(board, 150ms).pos;
            }
            auto end = std::chrono::steady_clock::now();
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "player " << parity  << " took " << elapsed << "ms" << std::endl;
            board.do_move(move);
            parity ^= 1;
        }
        std::cout << "----------\n";
        board.print();
        int score1 = board.count_player();
        int score2 = board.count_opponent();
        if (parity) std::swap(score1, score2);
        std::cout << "Engine 1: " << score1 << std::endl; 
        std::cout << "Engine 2: " << score2 << std::endl; 
    }
}
