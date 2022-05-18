#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <libataxx/position.hpp>
#include <sstream>
#include "../search/search.hpp"

namespace UAI {

namespace Extension {

auto perft(const libataxx::Position &pos, std::stringstream &stream) -> void;
auto split(const libataxx::Position &pos, std::stringstream &stream) -> void;
auto display(const libataxx::Position &pos) -> void;

}  // namespace Extension

auto uainewgame(libataxx::Position &pos) -> void;
auto isready() -> void;
auto setoption(std::stringstream &stream) -> void;
auto moves(libataxx::Position &pos, std::stringstream &stream) -> void;
auto position(libataxx::Position &pos, std::stringstream &stream) -> void;
auto go(const libataxx::Position &pos, std::stringstream &stream) -> void;
auto stop() -> void;
auto listen() -> void;
[[nodiscard]] auto search_options(std::stringstream &stream) -> SearchOptions;

}  // namespace UAI

#endif
