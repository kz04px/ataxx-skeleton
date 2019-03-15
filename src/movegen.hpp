#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <vector>
#include "move.hpp"
#include "position.hpp"

#define MAX_MOVES 256

int movegen(const Position &pos, Move *moves);

#endif
