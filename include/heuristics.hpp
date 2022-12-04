#pragma once

#include "board.hpp"

double reward[64] = {
    120, -20, 20, 5, 5, 20, -20, 120,
    -20, -40, -5, -5, -5, -5, -40, -20,
    20, -5, 15, 3, 3, 15, -5, 20,
    5, -5, 3, 3, 3, 3, -5, 5,
    5, -5, 3, 3, 3, 3, -5, 5,
    20, -5, 15, 3, 3, 15, -5, 20,
    -20, -40, -5, -5, -5, -5, -40, -20,
    120, -20, 20, 5, 5, 20, -20, 120
};

double greedy(Board& board) {
    return board.count_player() - board.count_opponent();
}

double mobility(Board &board) {
    double moves = __builtin_popcountll(board.get_moves());
    double opponent_moves = __builtin_popcountll(board.get_opponent_moves());
    return (moves - opponent_moves) / (moves + opponent_moves);
}

double reward_table(Board &board) {
    double ret = 0;
    for (int i = 0; i < 64; i++) {
        if (board.player & (1 << i))
            ret += reward[i];
        else if (board.opponent & (1 << i))
            ret -= reward[i];
    }
    return ret;
}
