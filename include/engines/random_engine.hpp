#pragma once

#include "board.hpp"
#include "engine.hpp"

class Random_Engine : public Engine {
public:
    long long evaluation(Board& board, int depth);
    Move get_move(Board &board, std::chrono::milliseconds time);
};
