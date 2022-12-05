/**
 * @file engine.hpp
 * @author Dion Ong
 * @brief A pure virtual base class for search engines
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "board.hpp"
#include "move.hpp"

#include <chrono>
#include <condition_variable>

class Engine {
public:
    int turn;
    double (*heuristic_function)(Board&);

    virtual double evaluation(Board& board, int depth) { return 0; }
    virtual Move get_move(Board& board, std::chrono::milliseconds time);
    void get_move_at_depth(uint64_t player, uint64_t opponent, int depth, Move* move);

protected:
    std::condition_variable cv;
    std::mutex engine_mutex;
};
