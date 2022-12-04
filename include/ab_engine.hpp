#pragma once

#include "board.hpp"
#include "engine.hpp"

class AB_Engine : public Engine {
public:
    double evaluation(Board board, int depth) = 0;
};
