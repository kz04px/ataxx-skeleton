#include <iostream>
#include <libataxx/position.hpp>
#include "protocol.hpp"

namespace UAI::Extension {

// Display the board
void display(const libataxx::Position &pos) {
    std::cout << pos << std::endl;
}

}  // namespace UAI::Extension
