#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <cstdint>
#include <vector>
#include "position.hpp"
#include "move.hpp"

enum SearchType
{
    Time = 0,
    Depth,
    Nodes,
    Infinite
};

struct SearchOptions
{
    int type = SearchType::Time;
    // Time search
    int btime = -1;
    int wtime = -1;
    int binc = -1;
    int winc = -1;
    int movestogo = -1;
    // Nodes search
    std::uint64_t nodes = -1;
    // Depth search
    int depth = -1;
};

struct SearchStats
{
    std::uint64_t nodes = 0ULL;
    std::uint64_t tt_hits = 0ULL;
    std::uint64_t tb_hits = 0ULL;
    int depth = 0;
    int seldepth = 0;
};

typedef std::vector<Move> PV;

void search(const Position &pos, const SearchOptions options);
bool legal_pv(const Position &pos, const PV &pv);

#endif
