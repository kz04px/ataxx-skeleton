#ifndef MOVE_HPP
#define MOVE_HPP

#include <iostream>
#include <cassert>

enum MoveType : bool
{
    Single = 0,
    Double
};
/*
const char *move_strings[] = {
    "a1", "a2", "a3",
    "a1a3", "a1b3", "a2c3"
};
*/
struct Move
{
    Move() : from_(Square::a1), to_(Square::a1) {}
    Move(int t) : from_(t), to_(t)
    {
        assert(0 <= t && t <= 48);
    }
    Move(int f, int t) : from_(f), to_(t)
    {
        assert(f != t);
        assert(0 <= f && f <= 48);
        assert(0 <= t && t <= 48);
        assert(std::abs((t%7) - (f%7)) > 1 || std::abs((t/7) - (f/7)) > 1);
    }
    int from() const {return from_;}
    int to() const {return to_;}
    int from_ = 0;
    int to_ = 0;
};

inline int move_type(const Move &move)
{
    if(move.from() == move.to()) {return MoveType::Single;}
    return MoveType::Double;
}

inline bool operator==(const Move &lhs, const Move &rhs)
{
    return lhs.to() == rhs.to() && lhs.from() == rhs.from();
}

inline bool operator!=(const Move &lhs, const Move &rhs)
{
    return lhs.to() != rhs.to() || lhs.from() != rhs.from();
}

inline std::ostream &operator<<(std::ostream &os, const Move &m)
{
    const int from = m.from();
    const int to = m.to();
    if(move_type(m) == MoveType::Single)
    {
        os << static_cast<char>((to%7)+'a')
           << static_cast<char>((to/7)+'1');
    }
    else
    {
        os << static_cast<char>((from%7)+'a')
           << static_cast<char>((from/7)+'1')
           << static_cast<char>((to%7)+'a')
           << static_cast<char>((to/7)+'1');
    }
    return os;
}

// Convert a string into a Move
// eg. "a1b3" ---> Move(Square::a1, Square::b3)
inline Move parse_san(const std::string str)
{
    if(str.length() == 2)
    {
        int x = str[0] - 'a';
        int y = str[1] - '1';
        int sq = 7*y + x;
        return Move(sq);
    }
    else if(str.length() == 4)
    {
        int x1 = str[0] - 'a';
        int y1 = str[1] - '1';
        int sq1 = 7*y1 + x1;
        int x2 = str[2] - 'a';
        int y2 = str[3] - '1';
        int sq2 = 7*y2 + x2;

        int dx = std::abs(x1 - x2);
        int dy = std::abs(y1 - y2);

        // We were just given a single jump in longhand notation
        // e.g. "b2b3" instead of "b3"
        if(dx <= 1 && dy <= 1)
        {
            return Move(sq2);
        }
        // Normal double jump
        else
        {
            return Move(sq1, sq2);
        }
    }
    else if(str == "0000")
    {
        return Move(Square::a1);
    }
    return Move(Square::a1);
}

#endif
