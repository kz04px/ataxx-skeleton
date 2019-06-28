#include "movegen.hpp"
#include <cassert>
#include <iostream>
#include "move.hpp"
#include "other.hpp"
#include "position.hpp"

// Generate all legal moves for the position specified
// At most MAX_MOVES can be generated
int movegen(const Position &pos, Move *moves) {
    assert(moves);
    assert(legal_position(pos));

    if (gameover(pos)) {
        return 0;
    }

    const bool us = pos.turn;
    const bool them = !us;
    const std::uint64_t pieces = pos.pieces[us];
    const std::uint64_t filled = pos.pieces[us] | pos.pieces[them] | pos.gaps;
    const std::uint64_t empty = Board::All ^ filled;
    int num_moves = 0;

    // Single moves
    std::uint64_t singles = adjacent(pieces) & empty;
    while (singles) {
        assert(num_moves < MAX_MOVES);
        const int to = lsbll(singles);
        moves[num_moves] = Move(to);
        assert(move_type(moves[num_moves]) == MoveType::Single);
        num_moves++;
        singles &= singles - 1;
    }

    // Double moves
    std::uint64_t copy = pieces;
    while (copy) {
        const int from = lsbll(copy);
        std::uint64_t destinations = double_moves(from) & empty;
        while (destinations) {
            assert(num_moves < MAX_MOVES);
            const int to = lsbll(destinations);
            moves[num_moves] = Move(from, to);
            assert(move_type(moves[num_moves]) == MoveType::Double);
            num_moves++;
            destinations &= destinations - 1;
        }

        copy &= copy - 1;
    }

    // Add nullmove if there's no alternative
    if (num_moves == 0) {
        moves[0] = nullmove;
        num_moves++;
    }

    for (int i = 0; i < num_moves; ++i) {
        if (!legal_move(pos, moves[i])) {
            print(pos);
            std::cout << "Move: " << moves[i] << std::endl;
        }
        assert(legal_move(pos, moves[i]));
    }

    assert(count_moves(pos) == num_moves);

    return num_moves;
}

// Returns the number of legal moves in a position
// This function does NOT generate any moves
int count_moves(const Position &pos) {
    assert(legal_position(pos));

    if (gameover(pos)) {
        return 0;
    }

    const bool us = pos.turn;
    const bool them = !us;
    const std::uint64_t pieces = pos.pieces[us];
    const std::uint64_t filled = pos.pieces[us] | pos.pieces[them] | pos.gaps;
    const std::uint64_t empty = Board::All ^ filled;
    int num_moves = 0;

    // Single moves
    const std::uint64_t singles = adjacent(pieces) & empty;
    num_moves += popcountll(singles);

    // Double moves
    std::uint64_t copy = pieces;
    while (copy) {
        const int from = lsbll(copy);
        const std::uint64_t destinations = double_moves(from) & empty;
        num_moves += popcountll(destinations);
        copy &= copy - 1;
    }

    // Nullmove
    if (num_moves == 0) {
        num_moves++;
    }

    assert(num_moves < MAX_MOVES);
    return num_moves;
}
