#include <cstdint>
#include "makemove.hpp"
#include "valid.hpp"

// Apply a move to the position
void makemove(Position &pos, const Move &move)
{
    assert(legal_board(pos));
    assert(legal_move(pos, move));

    const bool us = pos.turn;
    const bool them = !us;
    const int to = move.to();
    const int from = move.from();
    const std::uint64_t to_bb = 1ULL << to;
    const std::uint64_t from_bb = 1ULL << from;
    const std::uint64_t neighbours = single_moves(to);
    const std::uint64_t captured = neighbours & pos.pieces[them];

    // Remove and replace our stone
    pos.pieces[us] ^= from_bb | to_bb;

    // Flip any captured stones
    pos.pieces[them] ^= captured;
    pos.pieces[us] ^= captured;

    // Increment halfmoves
    pos.halfmoves++;

    // Increment fullmoves
    if(pos.turn == Side::White)
    {
        pos.fullmoves++;
    }

    // Reset halfmove counter on single or capturing moves
    if(to == from || captured)
    {
        pos.halfmoves = 0;
    }

    pos.turn = !pos.turn;

    assert(legal_board(pos));
}
