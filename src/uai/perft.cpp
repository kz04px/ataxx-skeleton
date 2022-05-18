#include <chrono>
#include <iostream>
#include <libataxx/position.hpp>
#include "protocol.hpp"

namespace UAI::Extension {

// Perform a perft search
void perft(const libataxx::Position &pos, std::stringstream &stream) {
    auto depth = 0;
    stream >> depth;
    if (depth < 1) {
        depth = 1;
    }

    std::uint64_t nodes = 0ULL;
    for (int i = 1; i <= depth; ++i) {
        const auto start = std::chrono::high_resolution_clock::now();
        nodes = pos.perft(i);
        const auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        std::cout << "info"
                  << " depth " << i << " nodes " << nodes << " time " << static_cast<int>(elapsed.count() * 1000)
                  << " nps " << static_cast<int>(nodes / elapsed.count()) << std::endl;
    }

    std::cout << "nodes " << nodes << std::endl;
}

}  // namespace UAI::Extension
