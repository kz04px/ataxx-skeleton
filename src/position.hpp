#ifndef POSITION_HPP
#define POSITION_HPP

#include <cassert>
#include <cstdint>
#include <string>

enum Board : std::uint64_t
{
    All = 0x1FFFFFFFFFFFFULL,
    FileA = 0x0040810204081ULL,
    FileB = 0x0081020408102ULL,
    FileC = 0x0102040810204ULL,
    FileD = 0x0204081020408ULL,
    FileE = 0x0408102040810ULL,
    FileF = 0x0810204081020ULL,
    FileG = 0x1020408102040ULL,
    Rank1 = 0x0ULL,
    Rank2 = 0x0ULL,
    Rank3 = 0x0ULL,
    Rank4 = 0x0ULL,
    Rank5 = 0x0ULL,
    Rank6 = 0x0ULL,
    Rank7 = 0x0ULL,
    NotFileA = 0x1fbf7efdfbf7eULL,
    NotFileB = 0x1f7efdfbf7efdULL,
    NotFileC = 0x1efdfbf7efdfbULL,
    NotFileD = 0x1dfbf7efdfbf7ULL,
    NotFileE = 0x1bf7efdfbf7efULL,
    NotFileF = 0x17efdfbf7efdfULL,
    NotFileG = 0x0fdfbf7efdfbfULL
};

enum Side : bool
{
    Black,
    White
};

// clang-format off
enum Square : int
{
    a1 = 0, b1, c1, d1, e1, f1, g1,
    a2, b2, c2, d2, e2, f2, g2,
    a3, b3, c3, d3, e3, f3, g3,
    a4, b4, c4, d4, e4, f4, g4,
    a5, b5, c5, d5, e5, f5, g5,
    a6, b6, c6, d6, e6, f6, g6,
    a7, b7, c7, d7, e7, f7, g7, None
};
// clang-format on

struct Position {
    std::uint64_t pieces[2] = {0ULL, 0ULL};
    std::uint64_t gaps = 0ULL;
    std::uint64_t hash = 0ULL;
    bool turn = Side::Black;
};

void set_fen(Position &pos, const std::string &fen);
std::string get_fen(const Position &pos);
std::uint64_t single_moves(const int sq);
std::uint64_t double_moves(const int sq);
bool gameover(const Position &pos);
bool can_move(const Position &pos);
std::uint64_t adjacent(const std::uint64_t bb);
bool legal_position(const Position &pos);
void print(const Position &pos);
inline std::uint64_t sq_to_bb(const int sq) {
    assert(sq >= 0);
    assert(sq < 49);
    return 1ULL << sq;
}
inline int sq_to_rank(const int sq) {
    assert(sq >= 0);
    assert(sq < 49);
    return sq / 7;
}
inline int sq_to_file(const int sq) {
    assert(sq >= 0);
    assert(sq < 49);
    return sq % 7;
}

#endif
