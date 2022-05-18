#include <chrono>
#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "protocol.hpp"

namespace UAI::Extension {

// Perform a split perft
auto split(const libataxx::Position &pos, std::stringstream &stream) -> void {
    int depth = 0;
    stream >> depth;
    if (depth < 1) {
        depth = 1;
    }

    std::uint64_t total_nodes = 0ULL;
    const auto moves = pos.legal_moves();

    for (const auto move : moves) {
        libataxx::Position npos = pos;
        npos.makemove(move);

        const std::uint64_t nodes = npos.perft(depth - 1);
        total_nodes += nodes;

        std::cout << move << " " << nodes << std::endl;
    }

    std::cout << "nodes " << total_nodes << std::endl;
}

}  // namespace UAI::Extension
