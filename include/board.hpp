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
    bool operator==(const Board board) const {
        return player == board.player and opponent == board.opponent;
    }
    bool operator<(const Board board) const {
        if (player != board.player)
            return player < board.player;
        return opponent < board.opponent;
    }
    
    int count_player() const;
    int count_opponent() const;
    bool is_terminal();
    void print() const;
    void randomize(int seed);
    uint64_t get_moves() const;
    uint64_t test_get_moves() const;
    uint64_t get_opponent_moves();
    Move do_move(int pos);
    void undo_move(const Move &move);
};
