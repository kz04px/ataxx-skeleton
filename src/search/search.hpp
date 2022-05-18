#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <chrono>
#include <cstdint>
#include <functional>
#include <libataxx/move.hpp>
#include <libataxx/position.hpp>
#include <optional>
#include <vector>

#define MAX_DEPTH 128
#define MATE_SCORE 100000

enum class SearchType
{
    Time = 0,
    Depth,
    Nodes,
    Movetime,
    Infinite
};

struct [[nodiscard]] SearchOptions {
    static SearchOptions from_time(const int bt, const int wt, const int bi, const int wi) {
        return SearchOptions{SearchType::Time, bt, wt, bi, wi};
    }

    static SearchOptions from_movetime(const int time) {
        return SearchOptions{SearchType::Movetime, .movetime = time};
    }

    static SearchOptions from_depth(const int ply) {
        return SearchOptions{SearchType::Depth, .depth = ply};
    }

    static SearchOptions from_nodes(const std::uint64_t n) {
        return SearchOptions{SearchType::Nodes, .nodes = n};
    }

    SearchType type = SearchType::Time;
    // Time search
    std::optional<int> btime = {};
    std::optional<int> wtime = {};
    std::optional<int> binc = {};
    std::optional<int> winc = {};
    std::optional<int> movestogo = {};
    // Movetime search
    std::optional<int> movetime = {};
    // Nodes search
    std::optional<std::uint64_t> nodes = {};
    // Depth search
    std::optional<int> depth = {};
};

struct [[nodiscard]] SearchController {
    volatile bool *stop;
    std::uint64_t max_nodes;
    std::chrono::high_resolution_clock::time_point end_time;
};

using PV = std::vector<libataxx::Move>;

struct [[nodiscard]] SearchStack {
    int ply = 0;
    PV pv;
};

struct [[nodiscard]] SearchStats {
    std::uint64_t nodes = 0ULL;
    std::uint64_t tt_hits = 0ULL;
    std::uint64_t tb_hits = 0ULL;
    int depth = 0;
    int seldepth = 0;
    int score = 0;
    std::chrono::milliseconds elapsed = std::chrono::milliseconds();
    PV pv;
};

[[nodiscard]] int minimax(SearchController &controller,
                          SearchStats &stats,
                          SearchStack *stack,
                          const libataxx::Position &pos,
                          const int depth);
[[nodiscard]] libataxx::Move search(const libataxx::Position &pos,
                                    const SearchOptions &options,
                                    volatile bool *stop,
                                    std::function<void(const SearchStats &)> info_handler);
[[nodiscard]] bool legal_pv(const libataxx::Position &pos, const PV &pv);

#endif
