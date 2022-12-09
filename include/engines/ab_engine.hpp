#pragma once

#include "board.hpp"
#include "engine.hpp"

#include <map>
#include <mutex>
#include <utility>

class AB_Engine : public Engine {
public:
    std::string name = "AB_Engine";
    std::map<Board, std::pair<long long, long long>> table[20];

    long long evaluation(Board& board, int depth);
    long long search(Board& board, long long alpha, long long beta, int depth, int turn);

private:
    std::mutex map_mutex;
};
