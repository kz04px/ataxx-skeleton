#ifndef OTHER_HPP
#define OTHER_HPP

#include <cstdint>
#include <cassert>

// Returns the number of trailing 0-bits in x, starting at the least
// significant bit position. If bb is 0, the result is undefined
inline int lsbll(const std::uint64_t bb)
{
    assert(bb);
    return __builtin_ctzll(bb);
}

// Returns the number of 1-bits in bb
inline int popcountll(const std::uint64_t bb)
{
    return __builtin_popcountll(bb);
}

#endif
