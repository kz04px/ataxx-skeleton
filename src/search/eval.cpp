#include "eval.hpp"
#include <libataxx/position.hpp>

[[nodiscard]] int eval(const libataxx::Position &pos) {
    const int num_us = pos.us().count();
    const int num_them = pos.them().count();
    return 100 * (num_us - num_them);
}
