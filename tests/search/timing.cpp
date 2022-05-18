#include <doctest/doctest.h>
#include <chrono>
#include <libataxx/position.hpp>
#include "../../src/search/search.hpp"

TEST_CASE("Timing") {
    auto info_handler = [](const SearchStats &) {
    };

    volatile bool stop = false;
    auto pos = libataxx::Position("startpos");

    for (const auto movetime : {1, 50, 300}) {
        const auto options = SearchOptions::from_movetime(movetime);
        const auto t0 = std::chrono::steady_clock::now();
        [[maybe_unused]] const auto move = search(pos, options, &stop, info_handler);
        const auto t1 = std::chrono::steady_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
        CHECK(dt.count() <= movetime + 10);
    }
}
