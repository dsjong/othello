#include "board.hpp"
#include "engines/ab_engine.hpp"
#include "engines/random_engine.hpp"
#include "heuristics.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    Board board;
    Random_Engine engine1;
    AB_Engine engine2;
    engine2.heuristic_function = &mobility;

    int parity = 0;
    while (!board.is_terminal()) {
        board.print();
        int move;
        if (parity == 0) {
            move = engine1.get_move(board, 150).pos;
        }
        else {
            move = engine2.get_move(board, 150).pos;
        }
        printf("player %d moves move %d\n", parity, move);
        board.do_move(move);
        parity ^= 1;
    }
    board.print();
}
