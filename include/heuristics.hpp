#pragma once

#include "board.hpp"

double greedy(Board& board) {
    return board.count_player() - board.count_opponent();
}

double mobility(Board &board) {
    double moves = __builtin_popcountll(board.get_moves());
    double opponent_moves = __builtin_popcountll(board.get_opponent_moves());
    return (moves - opponent_moves) / (moves + opponent_moves);
}
