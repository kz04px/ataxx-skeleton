#include <iostream>
#include <libataxx/position.hpp>
#include <thread>
#include "../options.hpp"
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

    const auto options = search_options(stream);

    search_thread = std::thread([pos, options]() {
        const auto bestmove = search(pos, options, &search_stop);
        std::cout << "bestmove " << bestmove << "\n";
    });
}

}  // namespace UAI
