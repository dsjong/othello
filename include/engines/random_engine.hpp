/**
 * @file random_engine.hpp
 * @author Dion Ong
 * @brief Class for random engine
 * @version 0.1
 * @date 2022-12-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "board.hpp"
#include "engine.hpp"

class Random_Engine : public Engine {
public:
    long long evaluation(Board& board, int depth);
    Move get_move(Board &board, std::chrono::milliseconds time);
};
