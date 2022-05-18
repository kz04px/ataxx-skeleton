#include <cassert>
#include <libataxx/position.hpp>
#include <limits>
#include "eval.hpp"
#include "search.hpp"

// Minimax algorithm (negamax)
[[nodiscard]] auto minimax(SearchController &controller,
                           SearchStats &stats,
                           SearchStack *stack,
                           const libataxx::Position &pos,
                           const int depth) -> int {
    assert(stack);
    assert(controller.stop);

    // Stop if asked
    if (*controller.stop) {
        return 0;
    } else if (stats.nodes >= controller.max_nodes) {
        return 0;
    } else if (std::chrono::high_resolution_clock::now() > controller.end_time) {
        return 0;
    }

    // Update seldepth stats
    stats.seldepth = std::max(stack->ply, stats.seldepth);

    // Return mate or draw scores if the game is over
    if (pos.gameover()) {
        const auto num_us = pos.us().count();
        const auto num_them = pos.them().count();

        if (num_us > num_them) {
            return MATE_SCORE - stack->ply;
        } else if (num_us < num_them) {
            return -MATE_SCORE + stack->ply;
        } else {
            return 0;
        }
    }

    // Make sure we stop searching
    if (depth == 0 || stack->ply >= MAX_DEPTH) {
        return eval(pos);
    }

    auto best_score = std::numeric_limits<int>::min();

    // Move generation
    const auto moves = pos.legal_moves();

    // Keeping track of the node count
    stats.nodes += moves.size();

    // Play every legal move and run negamax on the resulting position
    for (const auto move : moves) {
        (stack + 1)->pv.clear();

        auto npos = pos;
        npos.makemove(move);
        const auto score = -minimax(controller, stats, stack + 1, npos, depth - 1);

        if (score > best_score) {
            // Update PV
            stack->pv.clear();
            stack->pv.push_back(move);
            stack->pv.insert(stack->pv.begin() + 1, (stack + 1)->pv.begin(), (stack + 1)->pv.end());

            best_score = score;
        }
    }

    return best_score;
}
