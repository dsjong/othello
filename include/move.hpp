/**
 * @file move.hpp
 * @author Dion Ong
 * @brief Class for a single Othello move
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "common.hpp"

#include <iostream>

class Move{
public:
    int pos = -1;
    uint64_t flip = 0;

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << (char) ('a' + move.pos % 8);
        os << (char) ('1' + move.pos / 8);
        return os;
    }
};
