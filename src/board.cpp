#include "board.hpp"
#include "common.hpp"

#include <assert.h>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

int Board::count_player() const {
    return __builtin_popcountll(player);
}

int Board::count_opponent() const {
    return __builtin_popcountll(opponent);
}

int Board::count() const {
    return count_player() + count_opponent();
}

bool Board::is_terminal() {
    return !(get_moves() | get_opponent_moves());
}

void Board::print() const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (player & (1ull << i))
            std::cout << "O";
        else if (opponent & (1ull << i))
            std::cout << "X";
        else
            std::cout << ".";
        if ((i+1) % BOARD_LEN == 0)
            std::cout << "\n";
    }
}

/**
 * @brief Randomizes the board by playing 8 random moves
 * 
 * @param seed 
 */
void Board::randomize(int seed) {
    std::mt19937 rng(seed);
    for (int i = 0; i < 8; i++) {
        uint64_t moves = get_moves();
        int cnt = __builtin_popcountll(moves);
        if (moves) {
            std::uniform_int_distribution<uint64_t> distrib(0, cnt - 1);
            uint64_t idx = distrib(rng);
            for (int i = 0; i < idx; i++)
                moves -= moves & (-moves);
            do_move(__builtin_ctzll(moves));
        }
        else
            do_move(-1);
    }
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

uint64_t Board::get_opponent_moves() {
    std::swap(player, opponent);
    uint64_t ret = get_moves();
    std::swap(player, opponent);
    return ret;
}

uint64_t Board::get_frontier() const {
    uint64_t res = 0;
    uint64_t blank = ~(player | opponent);    
    res |= 0xfefefefefefefefe & (player << 1);
    res |= 0x7f7f7f7f7f7f7f7f & (player >> 1);
    res |= 0x7f7f7f7f7f7f7f00 & (player << 7);
    res |= 0x00fefefefefefefe & (player >> 7);
    res |= 0xffffffffffffff00 & (player << 8);
    res |= 0x00ffffffffffffff & (player >> 8);
    res |= 0xfefefefefefefe00 & (player << 9);
    res |= 0x007f7f7f7f7f7f7f & (player >> 9);
    return res & blank;
}

uint64_t Board::get_opponent_frontier() {
    std::swap(player, opponent);
    uint64_t ret = get_frontier();
    std::swap(player, opponent);
    return ret;
}

/**
 * @brief Flips discs after putting a token on pos, populating a Move object
 * 
 * @param pos 
 * @return Move object containing flipped discs 
 */
Move Board::do_move(int pos) {
    if (pos == -1) {
        std::swap(player, opponent);
        return Move{pos, 0};
    }
    uint64_t flip = 0;
    static const std::vector<std::pair<int, int>> dirs = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1},
    };
    auto get_bit = [&](uint64_t board, int i, int j){
        if (i < 0 || j < 0 || i >= 8 || j >= 8) return -1;
        int pos = i * 8 + j;
        return (int) ((board & (1ull << pos)) > 0);
    };
    int i = pos / 8, j = pos % 8;
    for (auto [di, dj] : dirs) {
        if (get_bit(opponent, i + di, j + dj) != 1)
            continue;
        uint64_t rev = 0;
        for (int x = i + di, y = j + dj; ; x += di, y += dj) {
            int p = get_bit(player, x, y);
            int o = get_bit(opponent, x, y);
            if (std::max(p, o) != 1) break;
            if (p == -1) break;
            if (p == 1) {
                flip |= rev;
                break;
            }
            else {
                rev |= 1ull << (8 * x + y);
            }
        }
    }

    Move move{pos, flip};
    player ^= move.flip;
    opponent ^= move.flip;
    player ^= 1ull << move.pos;
    std::swap(player, opponent);
    return move;
}

void Board::undo_move(const Move &move) {
    std::swap(player, opponent);
    if (move.pos == -1)
        return;
    player ^= 1ull << move.pos;
    player ^= move.flip;
    opponent ^= move.flip;
}
