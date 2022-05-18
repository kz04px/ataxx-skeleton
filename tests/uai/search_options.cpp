#include <doctest/doctest.h>
#include <sstream>
#include <string>
#include "../../src/uai/protocol.hpp"

TEST_CASE("UAI::search_options()") {
    SUBCASE("Time based search") {
        std::stringstream ss{"btime 1 wtime 2 binc 3 winc 4 movestogo 5"};
        const auto options = UAI::search_options(ss);
        REQUIRE(options.type == SearchType::Time);
        REQUIRE(options.btime == 1);
        REQUIRE(options.wtime == 2);
        REQUIRE(options.binc == 3);
        REQUIRE(options.winc == 4);
        REQUIRE(options.movestogo == 5);
    }

    SUBCASE("Movetime based search") {
        std::stringstream ss{"movetime 123"};
        const auto options = UAI::search_options(ss);
        REQUIRE(options.type == SearchType::Movetime);
        REQUIRE(options.movetime == 123);
    }

    SUBCASE("Depth based search") {
        std::stringstream ss{"depth 123"};
        const auto options = UAI::search_options(ss);
        REQUIRE(options.type == SearchType::Depth);
        REQUIRE(options.depth == 123);
    }

    SUBCASE("Nodes based search") {
        std::stringstream ss{"nodes 123"};
        const auto options = UAI::search_options(ss);
        REQUIRE(options.type == SearchType::Nodes);
        REQUIRE(options.nodes == 123);
    }

    SUBCASE("Infinite search") {
        std::stringstream ss{"infinite"};
        const auto options = UAI::search_options(ss);
        REQUIRE(options.type == SearchType::Infinite);
    }
}
