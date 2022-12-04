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
Move Engine::get_move(Board board, double time) {
    uint64_t moves = board.get_moves();
    double best_eval = -INF;
    Move best_move;
    for (; moves > 0; moves &= moves - 1) {
        Move move = board.do_move(moves & (-moves));
        if (evaluation(board, 5) >= best_eval) {
            best_move = move;
        }
        board.undo_move(move);
    }
    return best_move;
}
