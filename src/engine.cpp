#include "engine.hpp"
#include "board.hpp"
#include "move.hpp"

#include <assert.h>
#include <functional>
#include <thread>
#include <iostream>

/**
 * @brief Returns the best move for a particular board
 * 
 * @param board 
 * @param time 
 * @return Move object with populated pos and flip
 */
Move Engine::get_move(Board& board, std::chrono::milliseconds time) {
    turn = board.count() + 1;
    auto start = std::chrono::steady_clock::now();
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        board.undo_move(move);
        return move;
    }
    Move move;
    for (int depth = 1; depth < 15; depth++) {
        Move* cur_move = new Move;
        std::thread(&Engine::get_move_at_depth, this, board.player, board.opponent, depth, cur_move).detach();
        std::unique_lock<std::mutex> lk(engine_mutex);
        auto now = std::chrono::steady_clock::now();
        auto time_left = time - (now - start);
        if (!cv.wait_for(lk, time_left, [&]{ return cur_move->pos != -1; } )) {
            std::cout << "finished until depth " << depth - 1 << "\n";
            return move;
        }
        move = *cur_move;
    }
    return move;
}

void Engine::get_move_at_depth(uint64_t player, uint64_t opponent, int depth, Move* move) {
    Board board(player, opponent);
    uint64_t moves = board.get_moves();
    Move best_move;
    double best_eval = -INF-1;

    for (; moves > 0; moves -= moves & (-moves)) {
        Move move = board.do_move(__builtin_ctzll(moves));
        double score;
        score = -evaluation(board, depth);
        if (score > best_eval) {
            best_move = move;
            best_eval = score;
        }
        board.undo_move(move);
    }
    {
        std::lock_guard<std::mutex> lk(engine_mutex);
        *move = best_move;
    }
    cv.notify_all();
}
