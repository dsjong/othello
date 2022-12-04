/**
 * @file board.cpp
 * @author Dion Ong
 * @brief Class for an Othello board
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <common.hpp>
#include <move.hpp>

class Board {
public:
    uint64_t player;
    uint64_t opponent;

    Board() : player(INIT_PLAYER), opponent(INIT_OPPONENT) {}
    Board(uint64_t p, uint64_t o) : player(p), opponent(o) {}
    void print() const;
    void randomize(int seed);
    uint64_t get_moves() const;
    uint64_t test_get_moves() const;
    Move do_move(int pos);
    void undo_move(const Move &move);
};
