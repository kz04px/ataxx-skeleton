#include "position.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

// Look up tables for single and double moves from a given square
const std::uint64_t dist1[49] = {
    0x182,           0x385,           0x70a,          0xe14,
    0x1c28,          0x3850,          0x3020,         0xc103,
    0x1c287,         0x3850e,         0x70a1c,        0xe1438,
    0x1c2870,        0x181060,        0x608180,       0xe14380,
    0x1c28700,       0x3850e00,       0x70a1c00,      0xe143800,
    0xc083000,       0x3040c000,      0x70a1c000,     0xe1438000,
    0x1c2870000,     0x3850e0000,     0x70a1c0000,    0x604180000,
    0x1820600000,    0x3850e00000,    0x70a1c00000,   0xe143800000,
    0x1c287000000,   0x3850e000000,   0x3020c000000,  0xc1030000000,
    0x1c2870000000,  0x3850e0000000,  0x70a1c0000000, 0xe14380000000,
    0x1c28700000000, 0x1810600000000, 0x81800000000,  0x143800000000,
    0x287000000000,  0x50e000000000,  0xa1c000000000, 0x1438000000000,
    0x830000000000};
const std::uint64_t dist2[49] = {
    0x1c204,         0x3c408,         0x7c891,         0xf9122,
    0x1f2244,        0x1e0408,        0x1c0810,        0xe10204,
    0x1e20408,       0x3e44891,       0x7c89122,       0xf912244,
    0xf020408,       0xe040810,       0x70810207,      0xf102040f,
    0x1f224489f,     0x3e448913e,     0x7c891227c,     0x781020478,
    0x702040870,     0x3840810380,    0x7881020780,    0xf912244f80,
    0x1f224489f00,   0x3e448913e00,   0x3c081023c00,   0x38102043800,
    0x1c204081c000,  0x3c408103c000,  0x7c891227c000,  0xf912244f8000,
    0x1f224489f0000, 0x1e040811e0000, 0x1c081021c0000, 0x102040e00000,
    0x204081e00000,  0x448913e00000,  0x891227c00000,  0x112244f800000,
    0x20408f000000,  0x40810e000000,  0x102070000000,  0x2040f0000000,
    0x4489f0000000,  0x8913e0000000,  0x11227c0000000, 0x204780000000,
    0x408700000000};

// Set the current position from a given fen string
void set_fen(Position &pos, const std::string &fen) {
    if (fen == "startpos") {
        return set_fen(pos, "x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1");
    }

    // Clear board
    pos.pieces[Side::Black] = 0ULL;
    pos.pieces[Side::White] = 0ULL;
    pos.gaps = 0ULL;
    pos.hash = 0ULL;
    pos.turn = Side::Black;

    std::stringstream ss{fen};
    std::string word;
    int sq = Square::a7;

    // Board
    if (ss >> word) {
        for (const auto &c : word) {
            switch (c) {
                case 'x':
                case 'X':
                case 'b':
                case 'B':
                    pos.pieces[Side::Black] ^= sq_to_bb(sq);
                    sq++;
                    break;
                case 'o':
                case 'O':
                case 'w':
                case 'W':
                    pos.pieces[Side::White] ^= sq_to_bb(sq);
                    sq++;
                    break;
                case '-':
                    pos.gaps ^= sq_to_bb(sq);
                    sq++;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                    sq += c - '0';
                    break;
                case '/':
                    sq -= 14;
                    break;
                default:
                    return;
            }
        }
    }

    // Turn
    if (ss >> word) {
        if (word == "b" || word == "B" || word == "x" || word == "X") {
            pos.turn = Side::Black;
        } else {
            pos.turn = Side::White;
        }
    }
}

// Return a fen string from a given position
std::string get_fen(const Position &pos) {
    std::string fen = "";
    int empty = 0;

    // Board
    int sq = 42;
    while (sq >= 0) {
        const std::uint64_t bb = sq_to_bb(sq);

        if (bb & pos.pieces[Side::Black]) {
            if (empty) {
                fen += std::to_string(empty);
                empty = 0;
            }
            fen += "x";
        } else if (bb & pos.pieces[Side::White]) {
            if (empty) {
                fen += std::to_string(empty);
                empty = 0;
            }
            fen += "o";
        } else if (bb & pos.gaps) {
            if (empty) {
                fen += std::to_string(empty);
                empty = 0;
            }
            fen += "-";
        } else {
            empty += 1;
        }

        if (sq_to_file(sq) == 6) {
            sq -= 14;
            if (empty) {
                fen += std::to_string(empty);
                empty = 0;
            }
            if (sq >= -1) {
                fen += "/";
            }
        }
        sq++;
    }

    // Turn
    if (pos.turn == Side::Black) {
        fen += " b";
    } else {
        fen += " w";
    }

    return fen;
}

// Return look up table for single moves from a given square
std::uint64_t single_moves(const int sq) {
    assert(0 <= sq && sq <= 48);
    return dist1[sq];
}

// Return look up table for double moves from a given square
std::uint64_t double_moves(const int sq) {
    assert(0 <= sq && sq <= 48);
    return dist2[sq];
}

// Check if the game is over
// -- No black stones left
// -- No white stones left
// -- No moves left
bool gameover(const Position &pos) {
    const std::uint64_t pieces =
        pos.pieces[Side::Black] | pos.pieces[Side::White];
    const std::uint64_t empty = Board::All ^ (pieces | pos.gaps);
    const std::uint64_t moves = adjacent(adjacent(pieces));

    if (!pos.pieces[Side::Black]) {
        return true;
    }
    if (!pos.pieces[Side::White]) {
        return true;
    }
    if (moves & empty) {
        return false;
    }
    return true;
}

bool can_move(const Position &pos) {
    const std::uint64_t pieces =
        pos.pieces[Side::Black] | pos.pieces[Side::White];
    const std::uint64_t empty = Board::All ^ (pieces | pos.gaps);
    const std::uint64_t moves = adjacent(adjacent(pos.pieces[pos.turn]));

    return moves & empty;
}

std::uint64_t adjacent(const std::uint64_t bb) {
    return Board::All & (((bb >> 1) & (Board::NotFileG)) |  // Left 1
                         ((bb << 1) & (Board::NotFileA)) |  // Right 1
                         (bb << 7) |                        // Up 1
                         (bb >> 7) |                        // Down 1
                         ((bb << 8) & (Board::NotFileA)) |  // Up 1 right 1
                         ((bb << 6) & (Board::NotFileG)) |  // Up 1 left 1
                         ((bb >> 8) & (Board::NotFileG)) |  // Down 1 left 1
                         ((bb >> 6) & (Board::NotFileA)));  // Down 1 right 1
}

// Try to make sure the current position is legal
// -- No overlapping stones
// -- No stones on gaps
bool legal_position(const Position &pos) {
    if (pos.pieces[Side::White] & pos.pieces[Side::Black]) {
        return false;
    }
    if (pos.pieces[Side::White] & pos.gaps) {
        return false;
    }
    if (pos.pieces[Side::Black] & pos.gaps) {
        return false;
    }
    return true;
}

// Prints a simple ascii board to cout
void print(const Position &pos) {
    int sq = 42;
    while (sq >= 0) {
        const std::uint64_t bb = sq_to_bb(sq);

        if (bb & pos.pieces[Side::Black]) {
            std::cout << "x";
        } else if (bb & pos.pieces[Side::White]) {
            std::cout << "o";
        } else if (bb & pos.gaps) {
            std::cout << " ";
        } else {
            std::cout << "-";
        }

        if (sq_to_file(sq) == 6) {
            sq -= 14;
            std::cout << std::endl;
        }
        sq++;
    }
    std::cout << "Hash: " << pos.hash << std::endl;
    std::cout << "Turn: " << (pos.turn == Side::Black ? "b" : "w") << std::endl;
    std::cout << "FEN: " << get_fen(pos) << std::endl;
}
