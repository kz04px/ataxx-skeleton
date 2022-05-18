#include <iostream>
#include <libataxx/position.hpp>
#include "protocol.hpp"

namespace UAI::Extension {

// Display the board
auto display(const libataxx::Position &pos) -> void {
    std::cout << pos << std::endl;
}

}  // namespace UAI::Extension
