#include "engine.hpp"
#include "board.hpp"
#include "move.hpp"

#include <assert.h>
#include <functional>
#include <thread>

/**
 * @brief Returns the best move for a particular board
 * 
 * @param board 
 * @param time 
 * @return Move object with populated pos and flip
 */
Move Engine::get_move(Board& board, std::chrono::milliseconds time) {
    auto start = std::chrono::steady_clock::now();
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        board.undo_move(move);
        return move;
    }
    Move move{-1, 0};
    for (int depth = 1; depth < 10; depth++) {
        auto now = std::chrono::steady_clock::now();
        auto time_left = time - (now - start);
        std::unique_lock<std::mutex> lk(cv_m);
        std::thread t(&Engine::get_move_at_depth, this, std::ref(board), depth, std::ref(move));
        if (cv.wait_for(lk, time_left) == std::cv_status::timeout) {
            return move;
        }
    }
    assert(move.pos != -1);
    return move;
}

void Engine::get_move_at_depth(Board &board, int depth, Move &move) {
    std::lock_guard<std::mutex> lk(cv_m);
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
    cv.notify_all();
    move = best_move;
}
