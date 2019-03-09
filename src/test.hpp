#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include "position.hpp"
#include "perft.hpp"
#include "protocol.hpp"
#include "search.hpp"
#include "options.hpp"

bool test_fen()
{
    const std::string tests[] = {
        "x5o/7/2-1-2/7/2-1-2/7/o5x b 0 1"
    };
    for(const auto &fen : tests)
    {
        Position pos;
        set_fen(pos, fen);
        if(get_fen(pos) != fen)
        {
            return false;
        }
    }
    return true;
}

bool test_perft()
{
    const std::pair<std::string, std::uint64_t> tests[] = {
        {"x5o/7/2-1-2/7/2-1-2/7/o5x b 0 1", 14},
        {"x5o/7/7/7/7/7/o5x b 0 1", 16}
    };
    for(const auto &[fen, nodes] : tests)
    {
        Position pos;
        set_fen(pos, fen);
        if(nodes != perft(pos, 1))
        {
            return false;
        }
    }
    return true;
}

bool test_uai_pos()
{
    const std::pair<std::string, std::string> tests[] = {
        {"startpos", "x5o/7/2-1-2/7/2-1-2/7/o5x b 0 1"},
        {"startpos moves g2", "x5o/7/2-1-2/7/2-1-2/6x/o5x w 0 1"},
        {"startpos moves g2 a1a3", "x5o/7/2-1-2/7/o1-1-2/6x/6x b 1 2"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x", "x5o/7/2-1-2/7/2-1-2/7/o5x b 0 1"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w", "x5o/7/2-1-2/7/2-1-2/7/o5x w 0 1"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x b 2", "x5o/7/2-1-2/7/2-1-2/7/o5x b 2 1"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w 2 5", "x5o/7/2-1-2/7/2-1-2/7/o5x w 2 5"}
    };
    for(const auto &[input, fen] : tests)
    {
        Position pos;
        std::stringstream ss(input);
        UAI::position(pos, ss);
        if(get_fen(pos) != fen)
        {
            std::cout << fen << std::endl;
            return false;
        }
    }
    return true;
}

bool test_uai_moves()
{
    const std::pair<std::string, std::string> tests[] = {
        {"g2 a1a3", "x5o/7/2-1-2/7/o1-1-2/6x/6x b 1 2"}
    };
    for(const auto &[moves, fen] : tests)
    {
        Position pos;
        set_fen(pos, "startpos");
        std::stringstream s(moves);
        UAI::moves(pos, s);
        if(get_fen(pos) != fen)
        {
            std::cout << get_fen(pos) << std::endl;
            return false;
        }
    }
    return true;
}

bool test_options()
{
    // Spin
    Options::spins["TestSpin"] = Options::Spin(0, 10, 5);
    if(Options::spins["TestSpin"].get() != 5) {return false;}
    Options::set("TestSpin", "-2");
    if(Options::spins["TestSpin"].get() != 0) {return false;}
    Options::set("TestSpin", "12");
    if(Options::spins["TestSpin"].get() != 10) {return false;}

    // Check
    Options::checks["TestCheck"] = Options::Check(false);
    if(Options::checks["TestCheck"].get() != false) {return false;}
    Options::set("TestCheck", "true");
    if(Options::checks["TestCheck"].get() != true) {return false;}

    // String
    Options::strings["TestString"] = Options::String("Test");
    if(Options::strings["TestString"].get() != "Test") {return false;}
    Options::set("TestString", "Test2");
    if(Options::strings["TestString"].get() != "Test2") {return false;}

    // Combo
    // TODO

    // Spin -- Multiple word name/value
    Options::strings["Test String"] = Options::String("Test Value");
    if(Options::strings["Test String"].get() != "Test Value") {return false;}

    // Clear options
    Options::spins = {};
    Options::checks = {};
    Options::strings = {};
    Options::combos = {};

    return true;
}

bool test_uai_setoption()
{
    Options::strings["Test Name"] = Options::String("Replace");
    std::stringstream ss("name Test Name value Some Value");

    UAI::setoption(ss);

    if(Options::strings["Test Name"].get() != "Some Value") {return false;}

    // Clear options
    Options::spins = {};
    Options::checks = {};
    Options::strings = {};
    Options::combos = {};

    return true;
}

bool test_pv()
{
    const std::pair<std::string, PV> legal_tests[] = {
        {"startpos", {Move(Square::g2)}},
        {"startpos", {Move(Square::g2), Move(Square::a1, Square::b3)}}
    };
    for(const auto &[fen, pv] : legal_tests)
    {
        Position pos;
        set_fen(pos, fen);
        if(!legal_pv(pos, pv)) {return false;}
    }

    const std::pair<std::string, PV> illegal_tests[] = {
        {"startpos", {Move(Square::g1)}},
        {"startpos", {Move(Square::a2)}},
        {"startpos", {Move(Square::g2), Move(Square::g2)}},
        {"startpos", {Move(Square::g1, Square::e3)}}
    };
    for(const auto &[fen, pv] : illegal_tests)
    {
        Position pos;
        set_fen(pos, fen);
        if(legal_pv(pos, pv)) {return false;}
    }

    return true;
}

void test()
{
Options::print();
    std::cout << (test_fen()     ? "Y" : "N") << " -- FEN parsing" << std::endl;
    std::cout << (test_perft()   ? "Y" : "N") << " -- Perft" << std::endl;
    std::cout << (test_options() ? "Y" : "N") << " -- Options" << std::endl;
    std::cout << (test_pv()      ? "Y" : "N") << " -- PV" << std::endl;
    // UAI
    std::cout << (test_uai_pos()       ? "Y" : "N") << " -- UAI::position" << std::endl;
    std::cout << (test_uai_moves()     ? "Y" : "N") << " -- UAI::moves" << std::endl;
    std::cout << (test_uai_setoption() ? "Y" : "N") << " -- UAI::setoption" << std::endl;
}

#endif
