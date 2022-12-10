#pragma once

#include "board.hpp"
#include "engine.hpp"

#include <mutex>
#include <utility>

class AB_Engine : public Engine {
public:
    std::string name = "AB_Engine";

    long long evaluation(Board& board, int depth);
    long long search(Board& board, long long alpha, long long beta, int depth, int turn);

private:
    std::mutex map_mutex;
};
