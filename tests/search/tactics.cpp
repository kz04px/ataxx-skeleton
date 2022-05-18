#include <doctest/doctest.h>
#include <array>
#include <libataxx/position.hpp>
#include <string>
#include "../../src/search/search.hpp"

TEST_CASE("Tactics") {
    using test_type = std::pair<std::string, std::string>;

    auto info_handler = [](const SearchStats &) {
    };

    volatile bool stop = false;
    const auto options = SearchOptions::from_depth(3);

    SUBCASE("Win in 1 ply") {
        const std::array<test_type, 2> tests = {{
            {"7/7/7/3o3/7/7/x6 x 0 1", "a1c3"},
            {"7/7/7/3o3/7/7/x6 o 0 1", "d4b2"},
        }};

        for (const auto &[fen, bestmove] : tests) {
            auto pos = libataxx::Position(fen);
            const auto move = search(pos, options, &stop, info_handler);
            CHECK(static_cast<std::string>(move) == bestmove);
        }
    }

    SUBCASE("Strong move") {
        const std::array<test_type, 2> tests = {{
            {"5o1/7/7/5x1/4ooo/4o1o/4ooo x 0 1", "f4f2"},
            {"5x1/7/7/5o1/4xxx/4x1x/4xxx o 0 1", "f4f2"},
        }};

        for (const auto &[fen, bestmove] : tests) {
            auto pos = libataxx::Position(fen);
            const auto move = search(pos, options, &stop, info_handler);
            CHECK(static_cast<std::string>(move) == bestmove);
        }
    }
}
