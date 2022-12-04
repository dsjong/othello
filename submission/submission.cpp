/**
 * @file common.hpp
 * @author Dion Ong
 * @brief Common constants, libraries, and utils
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#define BOARD_SIZE      64
#define BOARD_LEN       8
#define INIT_PLAYER     0x0810000000
#define INIT_OPPONENT   0x1008000000
#define INF             1e9
/**
 * @file move.hpp
 * @author Dion Ong
 * @brief Class for a single Othello move
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */




class Move{
public:
    int pos;
    uint64_t flip;

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << (char) ('a' + move.pos % 8);
        os << (char) ('1' + move.pos / 8);
        return os;
    }
};
/**
 * @file board.cpp
 * @author Dion Ong
 * @brief Class for an Othello board
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */



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


double reward[64] = {
    120, -20, 20, 5, 5, 20, -20, 120,
    -20, -40, -5, -5, -5, -5, -40, -20,
    20, -5, 15, 3, 3, 15, -5, 20,
    5, -5, 3, 3, 3, 3, -5, 5,
    5, -5, 3, 3, 3, 3, -5, 5,
    20, -5, 15, 3, 3, 15, -5, 20,
    -20, -40, -5, -5, -5, -5, -40, -20,
    120, -20, 20, 5, 5, 20, -20, 120
};

double greedy(Board& board) {
    return board.count_player() - board.count_opponent();
}

double mobility(Board &board) {
    double moves = __builtin_popcountll(board.get_moves());
    double opponent_moves = __builtin_popcountll(board.get_opponent_moves());
    return (moves - opponent_moves) / (moves + opponent_moves);
}

double reward_table(Board &board) {
    double ret = 0;
    for (int i = 0; i < 64; i++) {
        if (board.player & (1 << i))
            ret += reward[i];
        else if (board.opponent & (1 << i))
            ret -= reward[i];
    }
    return ret;
}
/**
 * @file engine.hpp
 * @author Dion Ong
 * @brief A pure virtual base class for search engines
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */




class Engine {
public:
    double (*heuristic_function)(Board&);

    virtual double evaluation(Board& board, int depth) { return 0; }
    virtual Move get_move(Board& board, std::chrono::milliseconds time);
};



class AB_Engine : public Engine {
public:
    std::map<std::pair<Board, int>, std::pair<double, double>> table;

    double evaluation(Board& board, int depth);
    double search(Board& board, double alpha, double beta, int depth);
};


int Board::count_player() const {
    return __builtin_popcountll(player);
}

int Board::count_opponent() const {
    return __builtin_popcountll(opponent);
}

bool Board::is_terminal() {
    return get_moves() + get_opponent_moves() == 0;
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
 * @brief Randomizes the current board
 * 
 * @param seed 
 */
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

/**
 * @brief Unit test for get_moves
 * 
 * @return Same as get_moves 
 */
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

uint64_t Board::get_opponent_moves() {
    std::swap(player, opponent);
    uint64_t ret = get_moves();
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

/**
 * @brief Returns the best move for a particular board
 * 
 * @param board 
 * @param time 
 * @return Move object with populated pos and flip
 */
Move Engine::get_move(Board& board, std::chrono::milliseconds time) {
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        board.undo_move(move);
        return move;
    }
    double best_eval = -INF-1;
    Move best_move;
    for (int depth = 1; depth < 20; depth++) {
        for (; moves > 0; moves -= moves & (-moves)) {
            Move move = board.do_move(__builtin_ctzll(moves));
            if (-evaluation(board, 6) > best_eval) {
                best_move = move;
            }
            best_move = move;
            board.undo_move(move);
        }
    }
    return best_move;
}


double AB_Engine::evaluation(Board& board, int depth) {
    return search(board, -INF, INF, depth);
}

double AB_Engine::search(Board& board, double alpha, double beta, int depth) {
    std::pair<Board, int> key = {board, depth};
    if (table.count(key)) {
        auto [lower, upper] = table[key];
        if (lower >= beta)
            return lower;
        if (upper <= alpha)
            return upper;
        alpha = std::max(alpha, lower);
        beta = std::min(beta, upper);
    }
    if (board.is_terminal()) {
        int diff = board.count_player() - board.count_opponent();
        return ((diff > 0) - (diff < 0)) * INF;
    }
    if (depth == 0)
        return (*heuristic_function)(board);
    
    double val, a;
    val = -INF, a = alpha;
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        val = std::max(val, -search(board, -beta, -a, depth));
        a = std::max(a, val);
        board.undo_move(move);
    }
    for (; moves > 0 && val < beta; moves -= moves & (-moves)) {
        Move move = board.do_move(__builtin_ctzll(moves));
        val = std::max(val, -search(board, -beta, -a, depth - 1));
        a = std::max(a, val);
        board.undo_move(move);
    }
    if (val <= alpha) {
        table[key].second = val;
    }
    if (val > alpha && val < beta) {
        table[key].second = val;
        table[key].first = val;
    }
    if (val >= beta) {
        table[key].first = val;
    }
    // board.print();
    // printf("evaluation: %f\n\n", val);
    return val;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    AB_Engine engine;
    engine.heuristic_function = &reward_table;
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
        Move move = engine.get_move(board, 150);
        std::cout << move << " MSG " << player << " " << opponent << std::endl;
    }
}
