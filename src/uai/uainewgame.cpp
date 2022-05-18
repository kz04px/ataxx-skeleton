#include <libataxx/position.hpp>
#include "protocol.hpp"

namespace UAI {

// New game started
auto uainewgame(libataxx::Position &pos) -> void {
    pos.set_fen("startpos");
}

}  // namespace UAI
