#include <chrono>
#include <iostream>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include "../options.hpp"
#include "protocol.hpp"

namespace UAI {

// Apply a series of moves to the position
// -- moves a3 d2d4
auto moves(libataxx::Position &pos, std::stringstream &stream) -> void {
    std::string word;
    while (stream >> word) {
        auto move = libataxx::Move();
        try {
            move = libataxx::Move::from_uai(word);
        } catch (...) {
            if (Options::debug.value) {
                std::cout << "info string failed to parse move \"" << word << "\"" << std::endl;
            }
            continue;
        }

        if (pos.legal_move(move)) {
            pos.makemove(move);
        } else {
            if (Options::debug.value) {
                std::cout << "info string illegal move \"" << move << "\"" << std::endl;
            }
        }
    }
}

}  // namespace UAI
