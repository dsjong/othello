/**
 * @file mtdf_engine.hpp
 * @author Dion Ong
 * @brief Class for MTDF engine (inherits from AB_Engine)
 * @version 0.1
 * @date 2022-12-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "board.hpp"
#include "engines/ab_engine.hpp"

#include <map>
#include <mutex>
#include <utility>

class MTDF_Engine : public AB_Engine {
public:
    std::map<Board, int> best[64];
    long long evaluation(Board& board, int depth);
    long long search(Board& board, long long alpha, long long beta, int depth, int turn, int player);

private:
    std::mutex map_mutex;
    long long g = 0;
};
