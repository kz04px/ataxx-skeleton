#include <chrono>
#include <iostream>
#include <thread>
#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "options.hpp"
#include "perft.hpp"
#include "protocol.hpp"
#include "search.hpp"

std::thread search_thread;
volatile bool search_stop = false;

namespace UAI {
// New game started
void uainewgame(Position &pos) {
    // TODO:
    // -- Clear TT
    set_fen(pos, "startpos");
}

// Say that we're ready
void isready() {
    std::cout << "readyok" << std::endl;
}

// Set an option
void setoption(std::stringstream &stream) {
    std::string word = "";

    stream >> word;
    if (word != "name") {
        return;
    }

    // Collect option name
    std::string name = "";
    while (stream >> word && word != "value") {
        if (name != "") {
            name += " ";
        }
        name += word;
    }

    // Collect option value
    std::string value = "";
    while (stream >> word) {
        if (value != "") {
            value += " ";
        }
        value += word;
    }

    if (name != "" && value != "") {
        Options::set(name, value);
    }
}

// Apply a series of moves to the position
// -- moves a3 d2d4
void moves(Position &pos, std::stringstream &stream) {
    std::string word = "";
    while (stream >> word) {
        Move move;
        try {
            move = parse_san(word);
        } catch (...) {
            if (Options::checks["debug"].get()) {
                std::cout << "info failed to parse move \"" << word << "\""
                          << std::endl;
            }
            continue;
        }

        if (legal_move(pos, move)) {
            makemove(pos, move);
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info illegal move \"" << move << "\""
                          << std::endl;
            }
        }
    }
}

// Set the current position
// -- position startpos
// -- position startpos moves a3 d2d4
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1
// -- position fen x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1 moves a3 d2d4
void position(Position &pos, std::stringstream &stream) {
    std::string word = "";
    std::string fen = "";
    stream >> word;

    // Position
    if (word == "startpos") {
        fen = "startpos";
        stream >> word;
    } else if (word == "fen") {
        // Collect fen string
        std::string value = "";
        while (stream >> word && word != "moves") {
            if (fen != "") {
                fen += " ";
            }
            fen += word;
        }
    } else {
        if (Options::checks["debug"].get()) {
            std::cout << "info unknown UAI::position term \"" << word << "\""
                      << std::endl;
        }
        return;
    }

    set_fen(pos, fen);

    // Return if we didn't run into a move string
    if (word != "moves") {
        return;
    }

    moves(pos, stream);
}

// Perform a perft search
void perft(const Position &pos, std::stringstream &stream) {
    int depth = 0;
    stream >> depth;
    if (depth < 1) {
        depth = 1;
    }

    std::uint64_t nodes = 0ULL;
    for (int i = 1; i <= depth; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        nodes = perft(pos, i);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        std::cout << "info"
                  << " depth " << i << " nodes " << nodes << " time "
                  << static_cast<int>(elapsed.count() * 1000) << " nps "
                  << static_cast<int>(nodes / elapsed.count()) << std::endl;
    }

    std::cout << "nodes " << nodes << std::endl;
}

// Perform a split perft
void split(const Position &pos, std::stringstream &stream) {
    int depth = 0;
    stream >> depth;
    if (depth < 1) {
        depth = 1;
    }

    std::uint64_t total_nodes = 0ULL;
    Move moves[MAX_MOVES];
    int num_moves = movegen(pos, moves);
    for (int i = 0; i < num_moves; ++i) {
        Position npos = pos;
        makemove(npos, moves[i]);
        std::uint64_t nodes = perft(npos, depth - 1);
        total_nodes += nodes;

        std::cout << moves[i] << " " << nodes << std::endl;
    }
    std::cout << "nodes " << total_nodes << std::endl;
}

// Display the board
void display(const Position &pos) {
    print(pos);
}

// Start searching for a best move (threaded)
void go(const Position &pos, std::stringstream &stream) {
    stop();

    SearchOptions options;
    std::string word;

    while (stream >> word) {
        // Node search
        if (word == "nodes") {
            options.type = SearchType::Nodes;
            stream >> options.nodes;
        }
        // Depth search
        else if (word == "depth") {
            options.type = SearchType::Depth;
            stream >> options.depth;
        }
        // Infinite search
        else if (word == "infinite") {
            options.type = SearchType::Infinite;
        }
        // Movetime
        else if (word == "movetime") {
            options.type = SearchType::Movetime;
            stream >> options.movetime;
        }
        // Time search
        else if (word == "btime") {
            options.type = SearchType::Time;
            stream >> options.btime;
        } else if (word == "wtime") {
            options.type = SearchType::Time;
            stream >> options.wtime;
        } else if (word == "binc") {
            options.type = SearchType::Time;
            stream >> options.binc;
        } else if (word == "winc") {
            options.type = SearchType::Time;
            stream >> options.winc;
        } else if (word == "movestogo") {
            options.type = SearchType::Time;
            stream >> options.movestogo;
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info unknown UAI::go term \"" << word << "\""
                          << std::endl;
            }
        }
    }

    search_thread = std::thread(search, pos, options, &search_stop);
}

// The move that we were pondering about got played
void ponderhit() {
}

// Stop searching
void stop() {
    search_stop = true;
    if (search_thread.joinable()) {
        search_thread.join();
    }
    search_stop = false;
}

// Communicate with the UAI protocol (Universal Ataxx Interface)
// Based on the UCI protocol (Universal Chess Interface)
void listen() {
    Position pos;
    uainewgame(pos);

    std::cout << "id name AtaxxEngine" << std::endl;
    std::cout << "id author kz04px" << std::endl;

    // Create options
    Options::checks["debug"] = Options::Check(false);

    Options::print();

    std::cout << "uaiok" << std::endl;

    std::string word;
    std::string line;

    // Wait for isready before we do anything else
    // The engine might be opened just to check if it works
    while (true) {
        std::getline(std::cin, line);
        std::stringstream stream{line};
        stream >> word;

        if (word == "isready") {
            // TODO:
            // -- Initialise hash etc
            isready();
            break;
        } else if (word == "setoption") {
            setoption(stream);
        } else if (word == "quit") {
            return;
        }
    }

    // isready received, now we're ready to do something
    bool quit = false;
    while (!quit) {
        std::getline(std::cin, line);
        std::stringstream stream{line};
        stream >> word;

        if (word == "uainewgame") {
            uainewgame(pos);
        } else if (word == "isready") {
            isready();
        } else if (word == "perft") {
            perft(pos, stream);
        } else if (word == "split") {
            split(pos, stream);
        } else if (word == "position") {
            position(pos, stream);
        } else if (word == "moves") {
            moves(pos, stream);
        } else if (word == "go") {
            go(pos, stream);
        } else if (word == "ponderhit") {
            ponderhit();
        } else if (word == "stop") {
            stop();
        } else if (word == "print") {
            display(pos);
        } else if (word == "display") {
            display(pos);
        } else if (word == "quit") {
            break;
        } else {
            if (Options::checks["debug"].get()) {
                std::cout << "info unknown UAI command \"" << word << "\""
                          << std::endl;
            }
        }
    }

    stop();
}
}  // namespace UAI
