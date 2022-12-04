#include "ab_engine.hpp"
#include "board.hpp"

double AB_Engine::evaluation(Board board, int depth) {
    if (depth == 0)
        return (*heuristic_function)(board);
    return 0;
}