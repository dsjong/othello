#include "engines/ab_engine.hpp"
#include "board.hpp"
#include "common.hpp"

#include <climits>
#include <iostream>
#include <utility>

long long AB_Engine::evaluation(Board& board, int depth) {
    return search(board, -INF, INF, depth, board.count(), 1);
}

long long AB_Engine::search(Board& board, long long alpha, long long beta, int depth, int turn, int player) {
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (this->turn != turn)
            return player == 1 ? LONG_LONG_MIN : LONG_LONG_MAX;
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
        return ((diff > 0) - (diff < 0)) * INF_EVAL * player;
    }
    if (depth == 0)
        return (*heuristic_function)(board) * player;
    
    long long val, a, b;
    uint64_t moves = board.get_moves();
    if (player == 1) {
        val = -INF, a = alpha;
        if (moves == 0) {
            Move move = board.do_move(-1);
            val = std::max(val, search(board, a, beta, depth, turn, -player));
            a = std::max(a, val);
            board.undo_move(move);
        }
        else {
            for (; moves > 0 && val < beta; moves -= moves & (-moves)) {
                Move move = board.do_move(__builtin_ctzll(moves));
                val = std::max(val, search(board, a, beta, depth - 1, turn, -player));
                a = std::max(a, val);
                board.undo_move(move);
            }
        }
    }
    else {
        val = INF, b = beta;
        if (moves == 0) {
            Move move = board.do_move(-1);
            val = std::min(val, search(board, alpha, b, depth, turn, -player));
            b = std::min(b, val);
            board.undo_move(move);
        }
        else {
            for (; moves > 0 && val > alpha; moves -= moves & (-moves)) {
                Move move = board.do_move(__builtin_ctzll(moves));
                val = std::min(val, search(board, alpha, b, depth - 1, turn, -player));
                b = std::min(b, val);
                board.undo_move(move);
            }
        }
    }
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (val != LONG_LONG_MAX && val != LONG_LONG_MIN) {
            bool exists = table[depth].count(board);
            if (val <= alpha) {
                table[depth][board].second = val;
                if (!exists)
                    table[depth][board].first = -INF;
            }
            if (val > alpha && val < beta) {
                table[depth][board].second = val;
                table[depth][board].first = val;
            }
            if (val >= beta) {
                table[depth][board].first = val;
                if (!exists)
                    table[depth][board].second = INF;
            }
        }
    }
    return val;
}
