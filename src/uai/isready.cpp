#include <iostream>
#include "protocol.hpp"

namespace UAI {

// Say that we're ready
auto isready() -> void {
    std::cout << "readyok" << std::endl;
}

}  // namespace UAI
