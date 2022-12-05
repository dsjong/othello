using namespace std::chrono_literals;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    init_edge_table();
    AB_Engine engine;
    engine.heuristic_function = &heuristic1;
    int id, boardSize;
    std::cin >> id >> boardSize;
    while (true) {
        uint64_t player = 0, opponent = 0;
        for (int i = 0; i < 64; i++) {
            char c;
            std::cin >> c;
            if (c == '0')
                player |= 1ull << i;
            if (c == '1')
                opponent |= 1ull << i;
        }
        int actionCount;
        std::cin >> actionCount;
        while(actionCount--) {
            std::string action;
            std::cin >> action;
        }
        if (id)
            std::swap(player, opponent);
        Board board(player, opponent);
        Move move = engine.get_move(board, 145ms);
        std::cout << move << " MSG " << player << " " << opponent << std::endl;
    }
}
