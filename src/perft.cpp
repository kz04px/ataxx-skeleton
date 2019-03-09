#include "perft.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "makemove.hpp"

// Count the nodes in the tree from this position to a certain depth
std::uint64_t perft(const Position &pos, const int depth)
{
    if(depth == 1)
    {
        Move moves[MAX_MOVES];
        return movegen(pos, moves);
    }
    else if(depth <= 0)
    {
        return 1ULL;
    }

    std::uint64_t nodes = 0ULL;

    Move moves[MAX_MOVES];
    int num_moves = movegen(pos, moves);
    for(int i = 0; i < num_moves; ++i)
    {
        Position npos = pos;
        makemove(npos, moves[i]);
        nodes += perft(npos, depth-1);
    }

    return nodes;
}
