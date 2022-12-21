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
