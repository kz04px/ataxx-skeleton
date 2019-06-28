#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <sstream>
#include <string>
#include "makemove.hpp"
#include "movegen.hpp"
#include "options.hpp"
#include "perft.hpp"
#include "position.hpp"
#include "protocol.hpp"
#include "search.hpp"

bool test_fen() {
    const std::string tests[] = {"x5o/7/2-1-2/7/2-1-2/7/o5x b",
                                 "x5o/7/2-1-2/7/2-1-2/7/o5x w",
                                 "7/7/7/7/7/7/7 b",
                                 "7/7/7/7/7/7/7 w"};
    for (const auto &fen : tests) {
        Position pos;
        set_fen(pos, fen);
        if (get_fen(pos) != fen) {
            return false;
        }
    }
    return true;
}

bool test_perft() {
    const std::pair<std::string, std::vector<std::uint64_t>> tests[] = {
        {"startpos", {14, 196, 4184, 86528, 2266352}},
        {"x5o/7/7/7/7/7/o5x b", {16, 256, 6460, 155888, 4752668}},
        {"7/7/7/7/7/7/7 b", {0, 0, 0, 0}},
        {"7/7/7/7/ooooooo/ooooooo/xxxxxxx x", {1, 75, 249}},
        {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo o", {1, 75, 249}},
    };
    for (const auto &[fen, nodes] : tests) {
        Position pos;
        set_fen(pos, fen);

        for (unsigned int i = 0; i < nodes.size(); ++i) {
            if (nodes[i] != perft(pos, i + 1)) {
                return false;
            }
        }
    }
    return true;
}

bool test_uai_pos() {
    const std::pair<std::string, std::string> tests[] = {
        {"startpos", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"startpos moves g2", "x5o/7/2-1-2/7/2-1-2/6x/o5x w"},
        {"startpos moves g2 a1a3", "x5o/7/2-1-2/7/o1-1-2/6x/6x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w", "x5o/7/2-1-2/7/2-1-2/7/o5x w"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x b", "x5o/7/2-1-2/7/2-1-2/7/o5x b"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x w", "x5o/7/2-1-2/7/2-1-2/7/o5x w"},
        {"fen x5o/7/2-1-2/7/2-1-2/7/o5x moves g2",
         "x5o/7/2-1-2/7/2-1-2/6x/o5x w"}};
    for (const auto &[input, fen] : tests) {
        Position pos;
        std::stringstream ss(input);
        UAI::position(pos, ss);
        if (get_fen(pos) != fen) {
            return false;
        }
    }
    return true;
}

bool test_uai_moves() {
    const std::pair<std::string, std::string> tests[] = {
        {"g2 a1a3", "x5o/7/2-1-2/7/o1-1-2/6x/6x b"},
        {"", "x5o/7/2-1-2/7/2-1-2/7/o5x b"}};
    for (const auto &[moves, fen] : tests) {
        Position pos;
        set_fen(pos, "startpos");
        std::stringstream s(moves);
        UAI::moves(pos, s);
        if (get_fen(pos) != fen) {
            return false;
        }
    }
    return true;
}

bool test_options() {
    // Spin
    Options::spins["TestSpin"] = Options::Spin(0, 10, 5);
    if (Options::spins["TestSpin"].get() != 5) {
        return false;
    }
    Options::set("TestSpin", "-2");
    if (Options::spins["TestSpin"].get() != 0) {
        return false;
    }
    Options::set("TestSpin", "12");
    if (Options::spins["TestSpin"].get() != 10) {
        return false;
    }

    // Check
    Options::checks["TestCheck"] = Options::Check(false);
    if (Options::checks["TestCheck"].get() != false) {
        return false;
    }
    Options::set("TestCheck", "true");
    if (Options::checks["TestCheck"].get() != true) {
        return false;
    }

    // String
    Options::strings["TestString"] = Options::String("Test");
    if (Options::strings["TestString"].get() != "Test") {
        return false;
    }
    Options::set("TestString", "Test2");
    if (Options::strings["TestString"].get() != "Test2") {
        return false;
    }

    // Combo
    Options::combos["TestCombo"] = Options::Combo("A", {"A", "B", "C"});
    if (Options::combos["TestCombo"].get() != "A") {
        return false;
    }
    Options::set("TestCombo", "B");
    if (Options::combos["TestCombo"].get() != "B") {
        return false;
    }
    Options::set("TestCombo", "C");
    if (Options::combos["TestCombo"].get() != "C") {
        return false;
    }
    Options::set("TestCombo", "D");
    if (Options::combos["TestCombo"].get() != "C") {
        return false;
    }

    // Spin -- Multiple word name/value
    Options::strings["Test String"] = Options::String("Test Value");
    if (Options::strings["Test String"].get() != "Test Value") {
        return false;
    }

    // Clear options
    Options::spins = {};
    Options::checks = {};
    Options::strings = {};
    Options::combos = {};

    return true;
}

bool test_uai_setoption() {
    Options::strings["Test Name"] = Options::String("Replace");
    std::stringstream ss("name Test Name value Some Value");

    UAI::setoption(ss);

    if (Options::strings["Test Name"].get() != "Some Value") {
        return false;
    }

    // Clear options
    Options::spins = {};
    Options::checks = {};
    Options::strings = {};
    Options::combos = {};

    return true;
}

bool test_pv() {
    const std::pair<std::string, PV> legal_tests[] = {
        {"startpos", {Move(Square::g2)}},
        {"startpos", {Move(Square::g2), Move(Square::a1, Square::b3)}}};
    for (const auto &[fen, pv] : legal_tests) {
        Position pos;
        set_fen(pos, fen);
        if (!legal_pv(pos, pv)) {
            return false;
        }
    }

    const std::pair<std::string, PV> illegal_tests[] = {
        {"startpos", {Move(Square::g1)}},
        {"startpos", {Move(Square::a2)}},
        {"startpos", {Move(Square::g2), Move(Square::g2)}},
        {"startpos", {Move(Square::g1, Square::e3)}}};
    for (const auto &[fen, pv] : illegal_tests) {
        Position pos;
        set_fen(pos, fen);
        if (legal_pv(pos, pv)) {
            return false;
        }
    }

    return true;
}

bool test_gameover() {
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
        {"xxxxxxx/-------/-------/7/-------/-------/ooooooo", true}};
    for (const auto &[fen, expected] : tests) {
        Position pos;
        set_fen(pos, fen);
        if (gameover(pos) != expected) {
            return false;
        }

        if (expected) {
            Move moves[MAX_MOVES];
            const int num_moves = movegen(pos, moves);
            if (num_moves != 0) {
                return false;
            }
        }
    }
    return true;
}

bool test_legal_move() {
    const std::string fens[]{"startpos"};
    for (const auto &fen : fens) {
        Position pos;
        set_fen(pos, fen);
        Move moves[MAX_MOVES];
        int num_moves = movegen(pos, moves);

        for (int to = 0; to < 49; ++to) {
            for (int from = 0; from < 49; ++from) {
                int dx = std::abs(sq_to_file(to) - sq_to_file(from));
                int dy = std::abs(sq_to_rank(to) - sq_to_rank(from));

                Move move;
                if (dx < 2 && dy < 2) {
                    move = Move(to);
                } else {
                    move = Move(from, to);
                }

                bool legal = legal_move(pos, move);
                bool found = false;
                for (int i = 0; i < num_moves; ++i) {
                    if (move == moves[i]) {
                        found = true;
                        break;
                    }
                }

                if (legal != found) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool test_parse_san() {
    const std::vector<std::string> moves = {
        "b2", "a1b2", "a2b2", "a3b2", "b1b2", "b3b2", "c1b2", "c2b2", "c3b2"};
    for (unsigned int a = 0; a < moves.size() - 1; ++a) {
        for (unsigned int b = a + 1; b < moves.size(); ++b) {
            const Move move1 = parse_san(moves[a]);
            const Move move2 = parse_san(moves[b]);
            if (move1 != move2) {
                return false;
            }
        }
    }

    // Null move
    try {
        const auto move = parse_san("0000");
    } catch (...) {
        return false;
    }

    const std::vector<std::string> invalid = {"a8",
                                              "a0",
                                              "h1",
                                              "a2d8",
                                              "d0g6",
                                              "\n",
                                              "cat",
                                              "cats",
                                              "longlonglonglong"};
    for (const auto &str : invalid) {
        try {
            parse_san(str);
            return false;
        } catch (...) {
        }
    }

    return true;
}

bool test_nullmove() {
    // Nullmove legality
    const std::pair<std::string, bool> tests[] = {
        {"startpos", false},
        {"7/7/7/7/7/7/7 x", false},
        {"7/7/7/7/7/7/7 o", false},
        {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo x", false},
        {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo o", true},
        {"7/7/7/7/ooooooo/ooooooo/xxxxxxx x", true},
        {"7/7/7/7/ooooooo/ooooooo/xxxxxxx o", false},
        {"ooooooo/ooooooo/ooooooo/xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx x", false},
        {"ooooooo/ooooooo/ooooooo/xxxxxxx/xxxxxxx/xxxxxxx/xxxxxxx o", false},
    };

    for (const auto &[fen, legal] : tests) {
        Position pos;
        set_fen(pos, fen);

        // Does can_move() work?
        if (legal && can_move(pos)) {
            return false;
        }

        // Is nullmove legal?
        if (legal_move(pos, nullmove) != legal) {
            return false;
        }

        // Does movegen return a nullmove?
        if (legal) {
            Move moves[MAX_MOVES];
            const int num_moves = movegen(pos, moves);
            if (num_moves != 1 || moves[0] != nullmove) {
                return false;
            }
        }
    }

    // Nullmove equality
    if (nullmove != parse_san("0000")) {
        return false;
    }

    return true;
}

void test() {
    std::cout << std::boolalpha;
    std::cout << test_fen() << " -- FEN parsing" << std::endl;
    std::cout << test_perft() << " -- Perft" << std::endl;
    std::cout << test_options() << " -- Options" << std::endl;
    std::cout << test_pv() << " -- PV" << std::endl;
    std::cout << test_gameover() << " -- Gameover" << std::endl;
    std::cout << test_legal_move() << " -- Legal move" << std::endl;
    std::cout << test_parse_san() << " -- Parse san" << std::endl;
    std::cout << test_nullmove() << " -- Null move" << std::endl;
    // UAI
    std::cout << test_uai_pos() << " -- UAI::position" << std::endl;
    std::cout << test_uai_moves() << " -- UAI::moves" << std::endl;
    std::cout << test_uai_setoption() << " -- UAI::setoption" << std::endl;
}

#endif
