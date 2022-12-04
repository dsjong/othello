#include "engines/random_engine.hpp"

#include <chrono>
#include <random>
#include <vector>

Move Random_Engine::get_move(Board &board, double time) {
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        board.undo_move(move);
        return move;
    }
    std::vector<int> choices;
    for (; moves > 0; moves -= moves & (-moves)) {
        choices.push_back(__builtin_ctzll(moves));
    }

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    // std::mt19937 rng(69);
    std::uniform_int_distribution<int> distrib(0, choices.size() - 1);
    int idx = distrib(rng);
    Move move = board.do_move(choices[idx]);
    board.undo_move(move);
    return move;
}
