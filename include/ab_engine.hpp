#pragma once

#include "board.hpp"
#include "engine.hpp"

#include <map>
#include <utility>

class AB_Engine : public Engine {
public:
    std::map<std::pair<Board, int>, std::pair<double, double>> table;

    double evaluation(Board& board, int depth);
    double search(Board& board, double alpha, double beta, int depth);
};
