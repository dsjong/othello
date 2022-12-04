/**
 * @file engine.hpp
 * @author Dion Ong
 * @brief A pure virtual class for search engines
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "board.hpp"
#include "move.hpp"

class Engine {
public:
    double (*heuristic_function)(Board);

    virtual double evaluation(Board board, int depth) = 0;
    virtual Move get_move(Board board, double time);
};
