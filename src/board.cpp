#include "board.hpp"
#include "common.hpp"

#include <assert.h>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

void Board::print() const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (player & (1ll << i))
            std::cout << "O";
        else if (opponent & (1ll << i))
            std::cout << "X";
        else
            std::cout << ".";
        if ((i+1) % BOARD_LEN == 0)
            std::cout << "\n";
    }
}

void Board::randomize(int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<uint64_t> distrib(0, -1);
    uint64_t block = distrib(rng);
    uint64_t player = distrib(rng);
    uint64_t opponent = distrib(rng);
    this->player = player & (~block);
    this->opponent = opponent & (~block) & (~player);
}

/**
 * @brief Lists all legal moves of the current board
 * 
 * @return A bitboard where set bits are the legal moves 
 */
uint64_t Board::get_moves() const {
    uint64_t res = 0;
    uint64_t blank = ~(player | opponent);
    uint64_t t, w, v;
    auto get_mobility = [&](int shift) {
        t = w & (player >> shift);
        t |= w & (t >> shift); v = w & (w >> shift);
        t |= v & (t >> shift * 2); t |= v & (t >> shift * 2);
        res |= t >> shift;

        t = w & (player << shift);
        t |= w & (t << shift); v = w & (w << shift);
        t |= v & (t << shift * 2); t |= v & (t << shift * 2);
        res |= t << shift;
    };
    
    w = opponent & 0x7e7e7e7e7e7e7e7e;
    get_mobility(1);
    w = opponent & 0x00ffffffffffff00;
    get_mobility(8);
    w = opponent & 0x007e7e7e7e7e7e00;
    get_mobility(7);
    get_mobility(9);
    res &= blank;
    return res;
}

uint64_t Board::test_get_moves() const {
    uint64_t res = 0;
    uint64_t filled = player | opponent;
    std::vector<std::pair<int, int>> dirs = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1},
    };
    auto get_bit = [&](uint64_t board, int i, int j){
        if (i < 0 || j < 0 || i >= 8 || j >= 8) return -1;
        int pos = i * 8 + j;
        return (int) ((board & (1ull << pos)) > 0);
    };
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (get_bit(filled, i, j))
                continue;
            for (auto [di, dj] : dirs) {
                if (get_bit(opponent, i + di, j + dj) != 1)
                    continue;
                for (int x = i + di, y = j + dj; ; x += di, y += dj) {
                    int p = get_bit(player, x, y);
                    int o = get_bit(opponent, x, y);
                    if (std::max(p, o) != 1) break;
                    if (p == -1) break;
                    if (p == 1) {
                        res |= 1ull << (i * 8 + j);
                        goto end;
                    }
                }
            }
            end:;
        }
    }
    return res;
}

Move Board::do_move(int pos) {
    uint64_t flip = 0;
    Move move{pos, flip};
    player ^= move.flip;
    opponent ^= move.flip;
    player ^= 1ull << move.pos;
    std::swap(player, opponent);
    return move;
}

void Board::undo_move(const Move &move) {
    std::swap(player, opponent);
    player ^= 1ull << move.pos;
    player ^= move.flip;
    opponent ^= move.flip;
}
