#include "engine.hpp"
#include "board.hpp"
#include "move.hpp"

#include <utility>

/**
 * @brief Returns the best move for a particular board
 * 
 * @param board 
 * @param time 
 * @return Move object with populated pos and flip
 */
Move Engine::get_move(Board& board, double time) {
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        board.undo_move(move);
        return move;
    }
    double best_eval = -INF-1;
    Move best_move;
    for (; moves > 0; moves -= moves & (-moves)) {
        Move move = board.do_move(__builtin_ctzll(moves));
        if (-evaluation(board, 6) > best_eval) {
            best_move = move;
        }
        best_move = move;
        board.undo_move(move);
    }
    return best_move;
}
