#include "board.hpp"
#include "edgetable.hpp"
#include "engine.hpp"
#include "engines/ab_engine.hpp"
#include "engines/mtdf_engine.hpp"
#include "engines/random_engine.hpp"
#include "heuristics.hpp"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <map>
#include <vector>

using namespace std::chrono_literals;

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "usage: ./runner num-games engine1 engine2" << std::endl;
    }
    
    init_edge_table();
    int games = std::stoi(argv[1]);
    
    std::map<std::string, Engine*> engines;
    engines["random"] = new Random_Engine;
    engines["ab"] = new AB_Engine;
    engines["mtdf"] = new MTDF_Engine;

    double total1 = 0;
    double total2 = 0;
    for (int i = 0; i < games; i++) {
        for (int j = 0; j < 2; j++) {
            Board board;
            int parity = j;
            while (!board.is_terminal()) {
                auto start = std::chrono::steady_clock::now();
                #ifdef DEBUG
                board.print();
                #endif
                int move;
                std::string name;
                if (parity == 0) {
                    move = engines[argv[2]]->get_move(board, 150ms).pos;
                    name = argv[2];
                }
                else {
                    move = engines[argv[3]]->get_move(board, 150ms).pos;
                    name = argv[3];
                }
                #ifdef DEBUG
                auto end = std::chrono::steady_clock::now();
                int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                std::cout << name << " took " << elapsed << "ms out of 150ms" << std::endl;
                #endif
                board.do_move(move);
                parity ^= 1;
            }
            int score1 = board.count_player();
            int score2 = board.count_opponent();
            if (parity) std::swap(score1, score2);
            #ifdef DEBUG
            std::cout << "----------\n";
            board.print();
            #endif
            std::cout << argv[2] << ": " << score1 << std::endl;
            std::cout << argv[3] << ": " << score2 << std::endl;
            double points = (1 + (score1 > score2) - (score2 > score1) ) / 2.0;
            total1 += points;
            total2 += 1 - points;
        }
    }
    std::cout << "----------\n";
    std::cout << argv[2] << ": " << total1 << "\n";
    std::cout << argv[3] << ": " << total2 << "\n";
}
