#include <iostream>
#include "../options.hpp"
#include "protocol.hpp"

namespace UAI {

[[nodiscard]] auto search_options(std::stringstream &stream) -> SearchOptions {
    SearchOptions options;
    std::string word;

    while (stream >> word) {
        // Node search
        if (word == "nodes") {
            options.type = SearchType::Nodes;
            std::uint64_t n = 0;
            stream >> n;
            options.nodes = n;
        }
        // Depth search
        else if (word == "depth") {
            options.type = SearchType::Depth;
            int n = 0;
            stream >> n;
            options.depth = n;
        }
        // Infinite search
        else if (word == "infinite") {
            options.type = SearchType::Infinite;
        }
        // Movetime
        else if (word == "movetime") {
            options.type = SearchType::Movetime;
            int n = 0;
            stream >> n;
            options.movetime = n;
        }
        // Time search
        else if (word == "btime") {
            options.type = SearchType::Time;
            int n = 0;
            stream >> n;
            options.btime = n;
        } else if (word == "wtime") {
            options.type = SearchType::Time;
            int n = 0;
            stream >> n;
            options.wtime = n;
        } else if (word == "binc") {
            options.type = SearchType::Time;
            int n = 0;
            stream >> n;
            options.binc = n;
        } else if (word == "winc") {
            options.type = SearchType::Time;
            int n = 0;
            stream >> n;
            options.winc = n;
        } else if (word == "movestogo") {
            options.type = SearchType::Time;
            int n = 0;
            stream >> n;
            options.movestogo = n;
        } else {
            if (Options::debug.value) {
                std::cout << "info unknown UAI::go term \"" << word << "\"" << std::endl;
            }
        }
    }

    return options;
}

}  // namespace UAI
