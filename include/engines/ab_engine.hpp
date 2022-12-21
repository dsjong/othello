#pragma once

#include "board.hpp"
#include "engine.hpp"

#include <mutex>
#include <utility>

class AB_Engine : public Engine {
public:
    std::map<Board, std::pair<long long, long long>> table[20];

    long long evaluation(Board& board, int depth);
    long long search(Board& board, long long alpha, long long beta, int depth, int turn, int player);
    void get_move_at_depth(uint64_t player, uint64_t opponent, int depth, Move* move);

protected:
    std::mutex map_mutex;
};
