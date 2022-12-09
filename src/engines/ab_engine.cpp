#include "engines/ab_engine.hpp"
#include "board.hpp"
#include "common.hpp"

#include <iostream>
#include <utility>

long long AB_Engine::evaluation(Board& board, int depth) {
    return search(board, -INF, INF, depth, board.count());
}

long long AB_Engine::search(Board& board, long long alpha, long long beta, int depth, int turn) {
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (this->turn != turn) return 0;
        if (table[depth].count(board)) {
            auto [lower, upper] = table[depth][board];
            if (lower >= beta)
                return lower;
            if (upper <= alpha)
                return upper;
            alpha = std::max(alpha, lower);
            beta = std::min(beta, upper);
        }
    }
    
    if (board.is_terminal()) {
        int diff = board.count_player() - board.count_opponent();
        return ((diff > 0) - (diff < 0)) * INF_EVAL;
    }
    if (depth == 0)
        return (*heuristic_function)(board);
    
    long long val, a;
    val = -INF, a = alpha;
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        val = std::max(val, -search(board, -beta, -a, depth, turn));
        a = std::max(a, val);
        board.undo_move(move);
    }
    for (; moves > 0 && val < beta; moves -= moves & (-moves)) {
        Move move = board.do_move(__builtin_ctzll(moves));
        val = std::max(val, -search(board, -beta, -a, depth - 1, turn));
        a = std::max(a, val);
        board.undo_move(move);
    }
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (val <= alpha) {
            table[depth][board].second = val;
        }
        if (val > alpha && val < beta) {
            table[depth][board].second = val;
            table[depth][board].first = val;
        }
        if (val >= beta) {
            table[depth][board].first = val;
        }
    }
    return val;
}
