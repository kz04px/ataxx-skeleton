#ifndef PERFT_HPP
#define PERFT_HPP

/*
Startpos perft:
    depth 1 nodes 14
    depth 2 nodes 196
    depth 3 nodes 4184
    depth 4 nodes 86528
    depth 5 nodes 2266352
    depth 6 nodes 58227084
    depth 7 nodes 1777284300
    depth 8 nodes 53780731208
*/

#include <cstdint>
#include "position.hpp"

// Count the nodes in the tree from this position to a certain depth
std::uint64_t perft(const Position &pos, const int depth);

#endif
