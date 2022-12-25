class MTDF_Engine {
public:
    int turn;
    long long (*heuristic_function)(Board&) = &heuristic;
    Move get_move(Board& board, std::chrono::milliseconds time);
    void get_move_at_depth(uint64_t player, uint64_t opponent, int depth, Move* move);
    long long evaluation(Board& board, int depth);
    long long search(Board& board, long long alpha, long long beta, int depth, int turn, int player);
protected:
    std::condition_variable cv;
    std::mutex map_mutex;
    std::mutex engine_mutex;
    std::map<Board, std::pair<long long, long long>> table[20];
private:
    long long g = 0;
    std::map<Board, int> best[64];
};

Move MTDF_Engine::get_move(Board& board, std::chrono::milliseconds time) {
    turn = board.count();
    auto start = std::chrono::steady_clock::now();
    uint64_t moves = board.get_moves();
    if (moves == 0) {
        Move move = board.do_move(-1);
        board.undo_move(move);
        return move;
    }
    Move move;
    for (int depth = 3; depth < 14; depth++) {
        Move* cur_move = new Move;
        std::thread(&MTDF_Engine::get_move_at_depth, this, board.player, board.opponent, depth, cur_move).detach();
        std::unique_lock<std::mutex> lk(engine_mutex);
        auto now = std::chrono::steady_clock::now();
        auto time_left = time - (now - start);
        if (!cv.wait_for(lk, time_left, [&]{ return cur_move->pos != -1; } )) {
            #ifdef DEBUG
            std::cout << "finished until depth " << depth - 1 << std::endl;
            #endif
            return move;
        }
        move = *cur_move;
    }
    #ifdef DEBUG
    std::cout << "finished all depths\n";
    #endif
    return move;
}

void MTDF_Engine::get_move_at_depth(uint64_t player, uint64_t opponent, int depth, Move* move) {
    Board board(player, opponent);
    uint64_t moves = board.get_moves();
    long long score = evaluation(board, depth);
    Move best_move;
    for (; moves > 0; moves -= moves & (-moves)) {
        Move move = board.do_move(__builtin_ctzll(moves));
        long long lower;
        if (board.is_terminal()) {
            int diff = board.count_player() - board.count_opponent();
            lower = ((diff > 0) - (diff < 0)) * INF_EVAL * -1;
        }
        else {
            std::lock_guard<std::mutex> lk(engine_mutex);
            lower = table[depth - 1][board].first;
        }
        board.undo_move(move);
        if (lower == score) {
            best_move = move;
            break;
        }
    }
    {
        std::lock_guard<std::mutex> lk(engine_mutex);
        *move = best_move;
    }
    cv.notify_all();
}

long long MTDF_Engine::evaluation(Board& board, int depth) {
    long long upper = INF;
    long long lower = -INF;
    int turn = board.count();
    while (lower < upper) {
        long long beta = std::max(g, lower + 1);
        g = search(board, beta - 1, beta, depth, turn, 1);
        if (g < beta)
            upper = g;
        else
            lower = g;
    }
    return g;
}

long long MTDF_Engine::search(Board& board, long long alpha, long long beta, int depth, int turn, int player) {
    int best_move = -1;
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        if (this->turn != turn)
            return player == 1 ? LONG_LONG_MIN : LONG_LONG_MAX;
        if (table[depth].count(board)) {
            auto [lower, upper] = table[depth][board];
            if (lower >= beta)
                return lower;
            if (upper <= alpha)
                return upper;
            alpha = std::max(alpha, lower);
            beta = std::min(beta, upper);
        }
        if (depth > 0 && best[depth - 1].count(board))
            best_move = best[depth - 1][board];
    }
    
    if (board.is_terminal()) {
        int diff = board.count_player() - board.count_opponent();
        return ((diff > 0) - (diff < 0)) * INF_EVAL * player;
    }
    if (depth == 0)
        return (*heuristic_function)(board) * player;
    
    long long val, a, b;
    uint64_t moves = board.get_moves();
    if (player == 1) {
        val = -INF, a = alpha;
        if (moves == 0) {
            Move move = board.do_move(-1);
            val = std::max(val, search(board, a, beta, depth, turn, -player));
            a = std::max(a, val);
            board.undo_move(move);
        }
        else {
            auto try_move = [&](int cur_move) {
                moves -= 1ull << cur_move;
                Move move = board.do_move(cur_move);
                long long new_val = search(board, a, beta, depth - 1, turn, -player);
                if (new_val > val) {
                    val = new_val;
                    best_move = cur_move;
                }
                a = std::max(a, val);
                board.undo_move(move);
            };
            if (best_move != -1) {
                try_move(best_move);
                if (val >= beta)
                    goto fail;
            }
            while (moves > 0 && val < beta) {
                int cur_move = __builtin_ctzll(moves);
                try_move(cur_move);
            }
        }
    }
    else {
        val = INF, b = beta;
        if (moves == 0) {
            Move move = board.do_move(-1);
            val = std::min(val, search(board, alpha, b, depth, turn, -player));
            b = std::min(b, val);
            board.undo_move(move);
        }
        else {
            auto try_move = [&](int cur_move) {
                moves -= 1ull << cur_move;
                Move move = board.do_move(cur_move);
                long long new_val = search(board, alpha, b, depth - 1, turn, -player);
                if (new_val < val) {
                    val = new_val;
                    best_move = cur_move;
                }
                b = std::min(b, val);
                board.undo_move(move);
            };
            if (best_move != -1) {
                try_move(best_move);
                if (val <= alpha)
                    goto fail;
            }
            while (moves > 0 && val > alpha) {
                int cur_move = __builtin_ctzll(moves);
                try_move(cur_move);
            }
        }
    }
    fail:;
    {
        std::lock_guard<std::mutex> lk(map_mutex);
        best[depth][board] = best_move;
        if (val != LONG_LONG_MAX && val != LONG_LONG_MIN) {
            bool exists = table[depth].count(board);
            if (val <= alpha) {
                table[depth][board].second = val;
                if (!exists)
                    table[depth][board].first = -INF;
            }
            if (val > alpha && val < beta) {
                table[depth][board].second = val;
                table[depth][board].first = val;
            }
            if (val >= beta) {
                table[depth][board].first = val;
                if (!exists)
                    table[depth][board].second = INF;
            }
        }
    }
    return val;
}
