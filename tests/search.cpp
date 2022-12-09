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
}
