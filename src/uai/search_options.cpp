#include <iostream>
#include "../options.hpp"
#include "protocol.hpp"

namespace UAI {

[[nodiscard]] SearchOptions search_options(std::stringstream &stream) {
    SearchOptions options;
    std::string word;

    while (stream >> word) {
        // Node search
        if (word == "nodes") {
            options.type = SearchType::Nodes;
            stream >> options.nodes;
        }
        // Depth search
        else if (word == "depth") {
            options.type = SearchType::Depth;
            stream >> options.depth;
        }
        // Infinite search
        else if (word == "infinite") {
            options.type = SearchType::Infinite;
        }
        // Movetime
        else if (word == "movetime") {
            options.type = SearchType::Movetime;
            stream >> options.movetime;
        }
        // Time search
        else if (word == "btime") {
            options.type = SearchType::Time;
            stream >> options.btime;
        } else if (word == "wtime") {
            options.type = SearchType::Time;
            stream >> options.wtime;
        } else if (word == "binc") {
            options.type = SearchType::Time;
            stream >> options.binc;
        } else if (word == "winc") {
            options.type = SearchType::Time;
            stream >> options.winc;
        } else if (word == "movestogo") {
            options.type = SearchType::Time;
            stream >> options.movestogo;
        } else {
            if (Options::debug.value) {
                std::cout << "info unknown UAI::go term \"" << word << "\"" << std::endl;
            }
        }
    }

    return options;
}

}  // namespace UAI
