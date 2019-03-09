#ifndef VALID_HPP
#define VALID_HPP

#include "position.hpp"

// Try to make sure the current board is legal
// -- No overlapping stones
// -- No stones on gaps
inline bool legal_board(const Position &pos)
{
    if(pos.pieces[Side::White] & pos.pieces[Side::Black]) {return false;}
    if(pos.pieces[Side::White] & pos.gaps) {return false;}
    if(pos.pieces[Side::Black] & pos.gaps) {return false;}
    return true;
}

// Try to make sure the move is pseudolegal without regard for the board
// legal   -- a1b3
// illegal -- a1d7
// legal   -- Placing a stone onto a gap
// legal   -- Placing a stone onto another stone
inline bool pseudolegal_move(const Move &move)
{
    const int from = move.from();
    const int from_x = from%7;
    const int from_y = from/7;

    const int to = move.to();
    const int to_x = to%7;
    const int to_y = to/7;

    const int dx = abs(to_x - from_x);
    const int dy = abs(to_y - from_y);

    if(from_x < 0 || from_y > 6) {return false;}
    if(to_x < 0 || to_x > 6) {return false;}
    if(dx > 2 || dy > 2) {return false;}

    if(dx == 0 && dy == 0) {return true;}
    if(dx + dy == 1) {return false;}

    return true;
}

// Try to make sure the move is legal with regard for the board
// illegal -- a1d7
// illegal -- Placing a stone onto a gap
// illegal -- Placing a stone onto another stone
inline bool legal_move(const Position &pos, const Move &move)
{
    const int from = move.from();
    const int to = move.to();
    const std::uint64_t filled = pos.pieces[Side::Black] | pos.pieces[Side::White] | pos.gaps;

    // Make sure the destination square is empty
    if((1ULL << to) & filled) {return false;}

    // Single moves
    if(from == to)
    {
        return single_moves(to) & pos.pieces[pos.turn];
    }
    // Double moves
    else
    {
        return double_moves(to) & pos.pieces[pos.turn] & (1ULL << from);
    }
}

#endif
