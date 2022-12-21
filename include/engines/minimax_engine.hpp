/**
 * @file minimax_engine.hpp
 * @author Andrew Wang
 * @brief Class for minimax engine
 * @version 0.1
 * @date 2022-12-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "board.hpp"
#include "engine.hpp"

#include <mutex>
#include <utility>

class Minimax_Engine : public Engine {
public:
    long long evaluation(Board& board, int depth);
    long long search(Board& board, int depth, int turn, int player);

private:
    std::mutex map_mutex;
    long long g = 0;
};
