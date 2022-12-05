#pragma once

#include "board.hpp"
#include "common.hpp"
#include "edgetable.hpp"

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

double reward_table(Board& board) {
    double ret = 0;
    for (int i = 0; i < 64; i++) {
        if (board.player & (1 << i))
            ret += reward[i];
        else if (board.opponent & (1 << i))
            ret -= reward[i];
    }
    return ret;
}

double greedy(Board& board) {
    return board.count_player() - board.count_opponent();
}

double mobility(Board& board) {
    double moves = __builtin_popcountll(board.get_moves());
    double opponent_moves = __builtin_popcountll(board.get_opponent_moves());
    return (moves - opponent_moves) / (moves + opponent_moves);
}

double potential_mobility(Board& board) {
    double frontier = __builtin_popcountll(board.get_frontier());
    double opponent_frontier = __builtin_popcountll(board.get_opponent_frontier());
    if (frontier == opponent_frontier)
        return 0;
    return (frontier - opponent_frontier) / (frontier + opponent_frontier);
}

double stability(Board& board) {
    double res = 0;
    for (int i = 0; i < 4; i++) {
        int val = 0;
        for (int j : edges[i]) {
            val *= 3;
            if (board.player & (1ull << j)) {
                val += 1;
            }
            else if (board.opponent & (1ull << j)) {
                val += 2;
            }
        }
        res += edgeTable[val];
    }
    return res;
}

double heuristic1(Board& board) {
    int move_number = board.count() - 3;
    double esac = 312 + 6.24 * move_number;
    double cmac = move_number <= 25 ? 50 + 2 * move_number : 75 + move_number;
    double score = 0;
    score += esac * stability(board);
    score += cmac * mobility(board);
    score += 99 * potential_mobility(board);
    return score;
}
