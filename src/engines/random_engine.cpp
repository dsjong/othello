#include "engines/random_engine.hpp"

#include  <random>
#include <vector>

Move Random_Engine::get_move(Board &board, double time) {
    uint64_t moves = board.get_moves();
    std::vector<int> choices;
    for (; moves > 0; moves &= moves - 1) {
        choices.push_back(__builtin_clzll(moves & (-moves)));
    }

    std::mt19937 rng(69);
    std::uniform_int_distribution<int> distrib(0, choices.size() - 1);
    int idx = distrib(rng);
    Move move = board.do_move(choices[idx]);
    board.undo_move(move);
    return move;
}
