#include "engines/minimax_engine.hpp"
#include "board.hpp"
#include "common.hpp"

#include <climits>
#include <iostream>
#include <utility>

long long Minimax_Engine::evaluation(Board& board, int depth) {
    int turn = board.count();
    return search(board, depth, turn, 1);
}

long long Minimax_Engine::search(Board& board, int depth, int turn, int player) {
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (this->turn != turn)
            return player == 1 ? LONG_LONG_MIN : LONG_LONG_MAX;
    }
    
    if (board.is_terminal()) {
        int diff = board.count_player() - board.count_opponent();
        return ((diff > 0) - (diff < 0)) * INF_EVAL * player;
    }
    if (depth == 0)
        return (*heuristic_function)(board) * player;
    
    long long val;
    uint64_t moves = board.get_moves();
    if (player == 1) {
        val = -INF;
        if (moves == 0) {
            Move move = board.do_move(-1);
            val = std::max(val, search(board, depth, turn, -player));
            board.undo_move(move);
        }
        else {
            for (; moves > 0; moves -= moves & (-moves)) {
                Move move = board.do_move(__builtin_ctzll(moves));
                val = std::max(val, search(board, depth - 1, turn, -player));
                board.undo_move(move);
            }
        }
    }
    else {
        val = INF;
        if (moves == 0) {
            Move move = board.do_move(-1);
            val = std::min(val, search(board, depth, turn, -player));
            board.undo_move(move);
        }
        else {
            for (; moves > 0; moves -= moves & (-moves)) {
                Move move = board.do_move(__builtin_ctzll(moves));
                val = std::min(val, search(board, depth - 1, turn, -player));
                board.undo_move(move);
            }
        }
    }

    return val;
}
