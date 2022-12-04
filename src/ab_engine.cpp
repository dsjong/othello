#include "ab_engine.hpp"
#include "board.hpp"

#include <utility>

double AB_Engine::evaluation(Board& board, int depth) {
    return search(board, -INF, INF, depth);
}

double AB_Engine::search(Board& board, double alpha, double beta, int depth) {
    std::pair<Board, int> key = {board, depth};
    if (table.count(key)) {
        auto [lower, upper] = table[key];
        if (lower >= beta)
            return lower;
        if (upper <= alpha)
            return upper;
        alpha = std::max(alpha, lower);
        beta = std::min(beta, upper);
    }
    if (depth == 0)
        return (*heuristic_function)(board);
    
    double val, a, b;
    // TODO: fix the condition here
    if (true) {
        val = -INF, a = alpha;
        uint64_t moves = board.get_moves();
        for (; moves > 0 && val < beta; moves &= moves - 1) {
            Move move = board.do_move(moves & (-moves));
            val = std::max(val, search(board, a, beta, depth - 1));
            a = std::max(a, val);
            board.undo_move(move);
        }
    }
    else {
        val = INF, b = beta;
        uint64_t moves = board.get_moves();
        for(; moves > 0 && val > alpha; moves &= moves - 1) {
            Move move = board.do_move(moves & (-moves));
            val = std::min(val, search(board, alpha, b, depth - 1));
            b = std::min(b, val);
            board.undo_move(move);
        }
    }
    if (val <= alpha) {
        table[key].second = val;
    }
    if (val > alpha && val < beta) {
        table[key].second = val;
        table[key].first = val;
    }
    if (val >= beta) {
        table[key].first = val;
    }
    return val;
}
