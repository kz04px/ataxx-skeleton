#include <iostream>
#include <cassert>
#include "movegen.hpp"
#include "valid.hpp"
#include "other.hpp"

std::uint64_t single_jump(const std::uint64_t bb)
{
    return Board::All & (((bb>>1) & (Board::NotFileG)) | // Left 1
                         ((bb<<1) & (Board::NotFileA)) | // Right 1
                          (bb<<7) |                      // Up 1
                          (bb>>7) |                      // Down 1
                         ((bb<<8) & (Board::NotFileA)) | // Up 1 right 1
                         ((bb<<6) & (Board::NotFileG)) | // Up 1 left 1
                         ((bb>>8) & (Board::NotFileG)) | // Down 1 left 1
                         ((bb>>6) & (Board::NotFileA))); // Down 1 right 1
}

// Generate all legal moves for the positiono specified
// Maximum of 256 moves
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
    std::uint64_t singles = single_jump(pieces) & empty;
    while(singles)
    {
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

    assert(num_moves < 256);
    return num_moves;
}