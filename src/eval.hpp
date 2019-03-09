#ifndef EVAL_HPP
#define EVAL_HPP

#include "position.hpp"
#include "other.hpp"

// Return the evaluation of the position from the side to move's point of view
int eval(const Position &pos)
{
    const bool us = pos.turn;
    const bool them = !us;
    const int num_us = popcountll(pos.pieces[us]);
    const int num_them = popcountll(pos.pieces[them]);
    return num_us - num_them;
}

#endif
