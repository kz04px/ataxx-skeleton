#include "search.hpp"
#include <chrono>
#include "../options.hpp"

// Perform a search as specified in the options
[[nodiscard]] libataxx::Move search(const libataxx::Position &pos,
                                    const SearchOptions &options,
                                    volatile bool *stop,
                                    std::function<void(const SearchStats &)> info_handler) {
    assert(stop);

    int depth = MAX_DEPTH;
    const auto start_time = std::chrono::high_resolution_clock::now();
    SearchStats stats;
    SearchStack stack[MAX_DEPTH + 1];
    SearchController controller;
    controller.stop = stop;
    controller.max_nodes = std::numeric_limits<std::uint64_t>::max();
    controller.end_time = start_time + std::chrono::hours(1);

    switch (options.type) {
        case SearchType::Time: {
            int search_time = 0;

            // Calculate time usage
            if (pos.turn() == libataxx::Side::Black) {
                search_time = *options.btime / 30;
            } else {
                search_time = *options.wtime / 30;
            }

            // Minimum time
            if (search_time < 1) {
                search_time = 1;
            }
            assert(search_time > 0);

            controller.end_time = start_time + std::chrono::milliseconds(search_time);
            break;
        }
        case SearchType::Depth:
            depth = *options.depth;
            break;
        case SearchType::Nodes:
            controller.max_nodes = *options.nodes;
            break;
        case SearchType::Movetime:
            controller.end_time = start_time + std::chrono::milliseconds(*options.movetime);
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
        const int score = minimax(controller, stats, stack, pos, i);
        auto finish = std::chrono::high_resolution_clock::now();

        assert(-MATE_SCORE < score && score < MATE_SCORE);

        if (i > 1 && (*stop || stats.nodes >= controller.max_nodes ||
                      std::chrono::high_resolution_clock::now() > controller.end_time)) {
            break;
        }

        // Update our main pv
        stats.depth = i;
        stats.score = score;
        stats.pv = stack[0].pv;
        stats.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start_time);
        assert(legal_pv(pos, stats.pv));

        // Send info string
        info_handler(stats);
    }

    return stats.pv.at(0);
}

// Check the legality of a PV based on a given board
[[nodiscard]] bool legal_pv(const libataxx::Position &pos, const PV &pv) {
    libataxx::Position npos = pos;
    for (const auto &move : pv) {
        if (!npos.legal_move(move)) {
            return false;
        }
        npos.makemove(move);
    }
    return true;
}
