#pragma once

#include "board.hpp"
#include "engine.hpp"

class Random_Engine : public Engine {
public:
    Move get_move(Board &board, double time);
};
