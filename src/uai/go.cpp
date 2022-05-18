#include <chrono>
#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <thread>
#include "../options.hpp"
#include "../search/search.hpp"
#include "protocol.hpp"

namespace UAI {

std::thread search_thread;
volatile bool search_stop = false;

// Stop searching
void stop() {
    search_stop = true;
    if (search_thread.joinable()) {
        search_thread.join();
    }
    search_stop = false;
}

// Start searching for a best move (threaded)
void go(const libataxx::Position &pos, std::stringstream &stream) {
    stop();

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
            if (Options::checks["debug"].get()) {
                std::cout << "info unknown UAI::go term \"" << word << "\"" << std::endl;
            }
        }
    }

    search_thread = std::thread([pos, options]() {
        const auto bestmove = search(pos, options, &search_stop);
        std::cout << "bestmove " << bestmove << "\n";
    });
}

}  // namespace UAI
