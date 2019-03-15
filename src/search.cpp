#include "search.hpp"
#include <chrono>
#include <iostream>
#include "makemove.hpp"
#include "options.hpp"

// Perform a search as specified in the options
void search(const Position &pos,
            const SearchOptions options,
            volatile bool *stop) {
    assert(stop);

    int depth = MAX_DEPTH;
    PV pv;
    SearchController controller;
    SearchStats stats;
    SearchStack stack[MAX_DEPTH + 1];
    controller.stop = stop;

    switch (options.type) {
        case SearchType::Time:
            break;
        case SearchType::Depth:
            depth = options.depth;
            break;
        case SearchType::Nodes:
            break;
        case SearchType::Movetime:
            break;
        case SearchType::Infinite:
            break;
        default:
            break;
    }

    // Set stack
    for (int i = 0; i < MAX_DEPTH + 1; ++i) {
        stack[i].ply = i;
    }

    // Iterative deepening
    for (int i = 1; i <= depth; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        int score = minimax(controller, stats, stack, pos, i);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        assert(-MATE_SCORE < score && score < MATE_SCORE);

        if (i > 1 && *stop) {
            break;
        }

        // Update our main pv
        pv = stack[0].pv;
        assert(legal_pv(pos, pv));

        // Send info string
        std::cout << "info"
                  << " score cs " << score << " depth " << i << " seldepth "
                  << stats.seldepth << " time "
                  << static_cast<int>(elapsed.count() * 1000) << " nodes "
                  << stats.nodes;
        if (elapsed.count() > 0) {
            std::cout << " nps "
                      << static_cast<std::uint64_t>(stats.nodes /
                                                    elapsed.count());
        }
        if (pv.size() > 0) {
            std::cout << " pv";
            for (const auto &move : pv) {
                std::cout << " " << move;
            }
        }
        std::cout << std::endl;
    }

    // Send our best move
    if (Options::checks["Ponder"].get() && pv.size() >= 2) {
        std::cout << "bestmove " << pv.at(0) << "ponder " << pv.at(1)
                  << std::endl;
    } else if (pv.size() >= 1) {
        std::cout << "bestmove " << pv.at(0) << std::endl;
    } else {
        std::cout << "bestmove 0000" << std::endl;
    }
}

// Check the legality of a PV based on a given board
bool legal_pv(const Position &pos, const PV &pv) {
    Position npos = pos;
    for (const auto &move : pv) {
        if (!legal_move(npos, move)) {
            return false;
        }
        makemove(npos, move);
    }
    return true;
}
