/**
 * @file common.hpp
 * @author Dion Ong
 * @brief Common constants, libraries, and utils
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stdint.h>

#define BOARD_SIZE      64
#define BOARD_LEN       8
#define INIT_PLAYER     0x0810000000
#define INIT_OPPONENT   0x1008000000
#define INF             1'000'000'000'000
#define INF_EVAL        1'000'000'000

static const int edges[4][10] = {
    {9, 0, 1, 2, 3, 4, 5, 6, 7, 14},
    {9, 0, 8, 16, 24, 32, 40, 48, 56, 49},
    {54, 63, 55, 47, 39, 31, 23, 15, 7, 14},
    {49, 56, 57, 58, 59, 60, 61, 62, 63, 54}
};
