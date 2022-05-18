#include <chrono>
#include <iostream>
#include <libataxx/position.hpp>
#include <thread>
#include "../options.hpp"
#include "../search/search.hpp"
#include "protocol.hpp"

namespace UAI {

auto operator<<(std::ostream &os, const Options::Check &check) noexcept -> std::ostream & {
    os << "option name " << check.name;
    os << " type check";
    os << " default " << (check.value ? "true" : "false");
    return os;
}

auto operator<<(std::ostream &os, const Options::Spin &spin) noexcept -> std::ostream & {
    os << "option name " << spin.name;
    os << " type spin";
    os << " default " << spin.val;
    os << " min " << spin.min;
    os << " max " << spin.max;
    return os;
}

auto operator<<(std::ostream &os, const Options::Button &button) noexcept -> std::ostream & {
    os << "option name " << button.name;
    os << " type button";
    return os;
}

auto operator<<(std::ostream &os, const Options::String &string) noexcept -> std::ostream & {
    os << "option name " << string.name;
    os << " type string";
    os << " default " << string.value;
    return os;
}

auto operator<<(std::ostream &os, const Options::Combo &combo) noexcept -> std::ostream & {
    os << "option name " << combo.name;
    os << " type combo";
    os << " default " << combo.values[combo.idx];
    os << " options";
    for (const auto &str : combo.values) {
        os << " " << str;
    }
    return os;
}

// Communicate with the UAI protocol (Universal Ataxx Interface)
// Based on the UCI protocol (Universal Chess Interface)
auto listen() -> void {
    std::cout << "id name AtaxxEngine" << std::endl;
    std::cout << "id author kz04px" << std::endl;
    std::cout << Options::debug << std::endl;
    std::cout << "uaiok" << std::endl;

    std::string word;
    std::string line;

    // Wait for isready before we do anything else
    // The engine might be opened just to check if it works
    while (true) {
        std::getline(std::cin, line);
        std::stringstream stream{line};
        stream >> word;

        if (word == "isready") {
            isready();
            break;
        } else if (word == "setoption") {
            setoption(stream);
        } else if (word == "quit") {
            return;
        }
    }

    libataxx::Position pos;
    uainewgame(pos);

    // isready received, now we're ready to do something
    auto quit = false;
    while (!quit) {
        std::getline(std::cin, line);
        std::stringstream stream{line};
        stream >> word;

        if (word == "uainewgame") {
            uainewgame(pos);
        } else if (word == "isready") {
            isready();
        } else if (word == "perft") {
            Extension::perft(pos, stream);
        } else if (word == "split") {
            Extension::split(pos, stream);
        } else if (word == "position") {
            position(pos, stream);
        } else if (word == "moves") {
            moves(pos, stream);
        } else if (word == "go") {
            go(pos, stream);
        } else if (word == "stop") {
            stop();
        } else if (word == "print" || word == "display") {
            Extension::display(pos);
        } else if (word == "quit") {
            break;
        } else {
            if (Options::debug.value) {
                std::cout << "info unknown UAI command \"" << word << "\"" << std::endl;
            }
        }
    }

    stop();
}

}  // namespace UAI
