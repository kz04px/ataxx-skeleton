#include "makemove.hpp"
#include <cstdint>

// Apply a move to the position
void makemove(Position &pos, const Move &move) {
    assert(legal_position(pos));
    assert(legal_move(pos, move));

    const bool us = pos.turn;
    const bool them = !us;
    const int to = move.to();
    const int from = move.from();
    const std::uint64_t to_bb = sq_to_bb(to);
    const std::uint64_t from_bb = sq_to_bb(from);
    const std::uint64_t neighbours = single_moves(to);
    const std::uint64_t captured = neighbours & pos.pieces[them];

    // Remove and replace our stone
    pos.pieces[us] ^= from_bb | to_bb;

    // Flip any captured stones
    pos.pieces[them] ^= captured;
    pos.pieces[us] ^= captured;

    pos.turn = !pos.turn;

    assert(legal_position(pos));
}
