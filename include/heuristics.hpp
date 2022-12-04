#pragma once

#include "board.hpp"

double greedy(Board& board) {
    return board.count_player() - board.count_opponent();
}

double naive(Board &board) {
    return 0;
}
