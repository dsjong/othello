#include "engine.hpp"
#include "board.hpp"
#include "move.hpp"

/**
 * @brief Returns the best move for a particular board
 * 
 * @param board 
 * @param time 
 * @return Move object with populated pos and flip
 */
Move Engine::get_move(Board& board, std::chrono::milliseconds time) {
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        board.undo_move(move);
        return move;
    }
    Move ret;
    for (int depth = 1; depth < 7; depth++) {
        Move best_move = get_move_at_depth(board, depth);
        ret = best_move;
    }
    return ret;
}

Move Engine::get_move_at_depth(Board& board, int depth) {
    uint64_t moves = board.get_moves();
    Move best_move{-1, 0};
    double best_eval = -INF-1;
    for (; moves > 0; moves -= moves & (-moves)) {
        Move move = board.do_move(__builtin_ctzll(moves));
        double score = -evaluation(board, depth);
        if (score > best_eval) {
            best_move = move;
            best_eval = score;
        }
        board.undo_move(move);
    }
    return best_move;
}
