#include "engines/mtdf_engine.hpp"
#include "board.hpp"
#include "common.hpp"

#include <iostream>
#include <utility>

#define EPS 1e-9

double g = 0;

double MTDF_Engine::evaluation(Board& board, int depth) {
    double upper = INF;
    double lower = -INF;
    int turn = board.count();
    while (lower + EPS < upper) {
        double beta = std::max(g, lower + EPS);
        g = search(board, beta - EPS, beta, depth, turn);
        if (g < beta)
            upper = g;
        else
            lower = g;
    }
    return g;
}

double MTDF_Engine::search(Board& board, double alpha, double beta, int depth, int turn) {
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (this->turn != turn) return 0;
        if (table[depth].count(board)) {
            auto [lower, upper] = table[depth][board];
            if (lower >= beta)
                return lower;
            if (upper <= alpha)
                return upper;
            alpha = std::max(alpha, lower);
            beta = std::min(beta, upper);
        }
    }
    
    if (board.is_terminal()) {
        int diff = board.count_player() - board.count_opponent();
        return ((diff > 0) - (diff < 0)) * INF;
    }
    if (depth == 0)
        return (*heuristic_function)(board);
    
    double a;
    g = -INF, a = alpha;
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        g = std::max(g, -search(board, -beta, -a, depth, turn));
        a = std::max(a, g);
        board.undo_move(move);
    }
    for (; moves > 0 && g < beta; moves -= moves & (-moves)) {
        Move move = board.do_move(__builtin_ctzll(moves));
        g = std::max(g, -search(board, -beta, -a, depth - 1, turn));
        a = std::max(a, g);
        board.undo_move(move);
    }
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (g <= alpha) {
            table[depth][board].second = g;
        }
        if (g > alpha && g < beta) {
            table[depth][board].second = g;
            table[depth][board].first = g;
        }
        if (g >= beta) {
            table[depth][board].first = g;
        }
    }
    return g;
}
