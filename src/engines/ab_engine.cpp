#include "engines/ab_engine.hpp"
#include "board.hpp"
#include "common.hpp"

#include <iostream>
#include <utility>

double AB_Engine::evaluation(Board& board, int depth) {
    return search(board, -INF, INF, depth);
}

double AB_Engine::search(Board& board, double alpha, double beta, int depth) {
    std::pair<Board, int> key = {board, depth};
    if (table.count(key)) {
        auto [lower, upper] = table[key];
        if (lower >= beta)
            return lower;
        if (upper <= alpha)
            return upper;
        alpha = std::max(alpha, lower);
        beta = std::min(beta, upper);
    }
    if (board.is_terminal()) {
        int diff = board.count_player() - board.count_opponent();
        return ((diff > 0) - (diff < 0)) * INF;
    }
    if (depth == 0)
        return (*heuristic_function)(board);
    
    double val, a;
    val = -INF, a = alpha;
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        val = std::max(val, -search(board, -beta, -a, depth));
        a = std::max(a, val);
        board.undo_move(move);
    }
    for (; moves > 0 && val < beta; moves &= moves - 1) {
        Move move = board.do_move(__builtin_ctzll(moves));
        val = std::max(val, -search(board, -beta, -a, depth - 1));
        a = std::max(a, val);
        board.undo_move(move);
    }
    if (val <= alpha) {
        table[key].second = val;
    }
    if (val > alpha && val < beta) {
        table[key].second = val;
        table[key].first = val;
    }
    if (val >= beta) {
        table[key].first = val;
    }
    // board.print();
    // printf("evaluation: %f\n\n", val);
    return val;
}