#include <iostream>
#include <libataxx/position.hpp>
#include <thread>
#include "../options.hpp"
#include "protocol.hpp"

namespace UAI {

std::thread search_thread;
volatile bool search_stop = false;

auto info_handler(const SearchStats &stats) noexcept -> void {
    std::cout << "info";
    std::cout << " depth " << stats.depth;
    std::cout << " seldepth " << stats.seldepth;
    std::cout << " score cp " << stats.score;
    std::cout << " time " << stats.elapsed.count();
    std::cout << " nodes " << stats.nodes;
    if (stats.elapsed.count() > 0) {
        std::cout << " nps " << static_cast<std::uint64_t>(1'000 * stats.nodes / stats.elapsed.count());
    }
    if (!stats.pv.empty()) {
        std::cout << " pv";
        for (const auto &move : stats.pv) {
            std::cout << " " << move;
        }
    }
    std::cout << std::endl;
}

// Stop searching
auto stop() -> void {
    search_stop = true;
    if (search_thread.joinable()) {
        search_thread.join();
    }
    search_stop = false;
}

// Start searching for a best move (threaded)
auto go(const libataxx::Position &pos, std::stringstream &stream) -> void {
    stop();

    const auto options = search_options(stream);

    search_thread = std::thread([pos, options]() {
        const auto bestmove = search(pos, options, &search_stop, info_handler);
        std::cout << "bestmove " << bestmove << "\n";
    });
}

}  // namespace UAI
