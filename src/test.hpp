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
#include "movegen.hpp"
#include "valid.hpp"

bool test_fen()
{
    const std::string tests[] = {
        "x5o/7/2-1-2/7/2-1-2/7/o5x b"
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
    const std::pair<std::string, std::vector<std::uint64_t>> tests[] = {
        {"startpos", {14, 196, 4184, 86528}},
        {"x5o/7/7/7/7/7/o5x b", {16, 256, 6460, 155888}},
        {"7/7/7/7/7/7/7 b", {0, 0, 0, 0}}
    };
    for(const auto &[fen, nodes] : tests)
    {
        Position pos;
        set_fen(pos, fen);

        for(unsigned int i = 0; i < nodes.size(); ++i)
        {
            if(nodes[i] != perft(pos, i+1))
            {
                return false;
            }
        }
    }
    return true;
}

bool test_uai_pos()
{
    const std::pair<std::string, std::string> tests[] = {
        {"startpos", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"startpos moves g2", "x5o/7/2-1-2/7/2-1-2/6x/o5x w"},
        {"startpos moves g2 a1a3", "x5o/7/2-1-2/7/o1-1-2/6x/6x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w", "x5o/7/2-1-2/7/2-1-2/7/o5x w"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x b", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w", "x5o/7/2-1-2/7/2-1-2/7/o5x w"}
    };
    for(const auto &[input, fen] : tests)
    {
        Position pos;
        std::stringstream ss(input);
        UAI::position(pos, ss);
        if(get_fen(pos) != fen)
        {
            return false;
        }
    }
    return true;
}

bool test_uai_moves()
{
    const std::pair<std::string, std::string> tests[] = {
        {"g2 a1a3", "x5o/7/2-1-2/7/o1-1-2/6x/6x b"}
    };
    for(const auto &[moves, fen] : tests)
    {
        Position pos;
        set_fen(pos, "startpos");
        std::stringstream s(moves);
        UAI::moves(pos, s);
        if(get_fen(pos) != fen)
        {
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

bool test_gameover()
{
    const std::pair<std::string, bool> tests[] = {
        {"startpos", false},
        {"7/7/7/7/7/7/7", true},
        {"7/7/7/7/7/7/x6", true},
        {"7/7/7/7/7/7/o6", true},
        {"7/7/7/7/7/7/ox5", false},
        {"xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx", true},
        {"ooooooo/ooooooo/ooooooo/ooooooo/ooooooo/ooooooo/ooooooo", true},
        {"xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx/ooooooo", true},
        {"ooooooo/ooooooo/ooooooo/ooooooo/ooooooo/ooooooo/xxxxxxx", true},
        {"-------/-------/-------/-------/-------/-------/-------", true},
        {"-------/-------/-------/-------/-------/-------/-----xo", true},
        {"-------/-------/-------/-------/-------/-------/-----xx", true},
        {"-------/-------/-------/-------/-------/-------/-----oo", true},
        {"xxxxxxx/-------/-------/7/-------/-------/ooooooo", true}
    };
    for(const auto &[fen, expected] : tests)
    {
        Position pos;
        set_fen(pos, fen);
        if(gameover(pos) != expected) {return false;}
    }
    return true;
}

bool test_legal_move()
{
    const std::string fens[] {
        "startpos"
    };
    for(const auto &fen : fens)
    {
        Position pos;
        set_fen(pos, fen);
        Move moves[MAX_MOVES];
        int num_moves = movegen(pos, moves);

        for(int to = 0; to < 49; ++to)
        {
            for(int from = 0; from < 49; ++from)
            {
                Move move = Move(from, to);
                bool legal = legal_move(pos, move);
                bool found = false;
                for(int i = 0; i < num_moves; ++i)
                {
                    if(move == moves[i])
                    {
                        found = true;
                        break;
                    }
                }

                if(legal != found) {return false;}
            }
        }
    }
    return true;
}

void test()
{
    std::cout << (test_fen()        ? "Y" : "N") << " -- FEN parsing" << std::endl;
    std::cout << (test_perft()      ? "Y" : "N") << " -- Perft" << std::endl;
    std::cout << (test_options()    ? "Y" : "N") << " -- Options" << std::endl;
    std::cout << (test_pv()         ? "Y" : "N") << " -- PV" << std::endl;
    std::cout << (test_gameover()   ? "Y" : "N") << " -- Gameover" << std::endl;
    std::cout << (test_legal_move() ? "Y" : "N") << " -- Legal move" << std::endl;
    // UAI
    std::cout << (test_uai_pos()       ? "Y" : "N") << " -- UAI::position" << std::endl;
    std::cout << (test_uai_moves()     ? "Y" : "N") << " -- UAI::moves" << std::endl;
    std::cout << (test_uai_setoption() ? "Y" : "N") << " -- UAI::setoption" << std::endl;
}

#endif
