#include <libataxx/position.hpp>
#include "protocol.hpp"

namespace UAI {

// New game started
void uainewgame(libataxx::Position &pos) {
    pos.set_fen("startpos");
}

}  // namespace UAI
