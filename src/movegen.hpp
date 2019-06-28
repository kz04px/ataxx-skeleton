#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <vector>

#define MAX_MOVES 256

struct Move;
struct Position;

int movegen(const Position &pos, Move *moves);
int count_moves(const Position &pos);

#endif
