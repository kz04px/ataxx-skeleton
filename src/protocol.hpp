#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <sstream>
#include "position.hpp"

namespace UAI {
void uainewgame(Position &pos);
void isready();
void setoption(std::stringstream &stream);
void moves(Position &pos, std::stringstream &stream);
void position(Position &pos, std::stringstream &stream);
void perft(const Position &pos, std::stringstream &stream);
void split(const Position &pos, std::stringstream &stream);
void display(const Position &pos);
void go(const Position &pos, std::stringstream &stream);
void ponderhit();
void stop();
void listen();
}  // namespace UAI

#endif
