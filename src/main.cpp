#include "board.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    Board board;
    board.player = 1;
    board.opponent = 30;
    board.print();
    board.do_move(5);
    board.print();
}
