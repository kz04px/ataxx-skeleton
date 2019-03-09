#include <iostream>
#include <cassert>
#include "movegen.hpp"
#include "valid.hpp"
#include "other.hpp"

// Generate all legal moves for the positiono specified
// At most MAX_MOVES can be generated
int movegen(const Position &pos, Move *moves)
{
    assert(moves);
    assert(legal_board(pos));

    const bool us = pos.turn;
    const bool them = !us;
    const std::uint64_t pieces = pos.pieces[us];
    const std::uint64_t filled = pos.pieces[us] | pos.pieces[them] | pos.gaps;
    const std::uint64_t empty = Board::All ^ filled;
    int num_moves = 0;

    // Single moves
    std::uint64_t singles = adjacent(pieces) & empty;
    while(singles)
    {
        assert(num_moves < MAX_MOVES);
        const int to = lsbll(singles);
        moves[num_moves] = Move(to);
        num_moves++;
        singles &= singles - 1;
    }

    // Double moves
    std::uint64_t copy = pieces;
    while(copy)
    {
        const int from = lsbll(copy);
        std::uint64_t destinations = double_moves(from) & empty;
        while(destinations)
        {
            assert(num_moves < MAX_MOVES);
            const int to = lsbll(destinations);
            moves[num_moves] = Move(from, to);
            num_moves++;
            destinations &= destinations - 1;
        }

        copy &= copy - 1;
    }

    for(int i = 0; i < num_moves; ++i)
    {
        assert(legal_move(pos, moves[i]));
    }

    return num_moves;
}
