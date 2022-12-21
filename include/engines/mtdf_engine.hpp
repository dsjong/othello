#pragma once

#include "engines/ab_engine.hpp"
#include "board.hpp"

#include <mutex>
#include <utility>

class MTDF_Engine : public AB_Engine {
public:
    long long evaluation(Board& board, int depth);
    long long search(Board& board, long long alpha, long long beta, int depth, int turn, int player);

private:
    std::mutex map_mutex;
    long long g = 0;
};
