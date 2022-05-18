#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <libataxx/position.hpp>
#include <sstream>

namespace UAI {

namespace Extension {

void perft(const libataxx::Position &pos, std::stringstream &stream);
void split(const libataxx::Position &pos, std::stringstream &stream);
void display(const libataxx::Position &pos);

}  // namespace Extension

void uainewgame(libataxx::Position &pos);
void isready();
void setoption(std::stringstream &stream);
void moves(libataxx::Position &pos, std::stringstream &stream);
void position(libataxx::Position &pos, std::stringstream &stream);
void go(const libataxx::Position &pos, std::stringstream &stream);
void stop();
void listen();

}  // namespace UAI

#endif
