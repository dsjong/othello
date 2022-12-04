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

class Engine {
public:
    double (*heuristic_function)(Board&);

    virtual double evaluation(Board& board, int depth) { return 0; }
    virtual Move get_move(Board& board, std::chrono::milliseconds time);
    Move get_move_at_depth(Board& board, int depth);
};
