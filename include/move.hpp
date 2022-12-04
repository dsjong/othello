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

class Move{
public:
    int pos;
    uint64_t flip;
};
