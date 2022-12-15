#include "engines/random_engine.hpp"

#include <random>
#include <vector>

long long Random_Engine::evaluation(Board& board, int depth) {
    return 0;
}

Move Random_Engine::get_move(Board &board, std::chrono::milliseconds time) {
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
    std::uniform_int_distribution<int> distrib(0, choices.size() - 1);
    int idx = distrib(rng);
    Move move = board.do_move(choices[idx]);
    board.undo_move(move);
    return move;
}
