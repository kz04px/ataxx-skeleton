#include "eval.hpp"
#include <libataxx/position.hpp>

[[nodiscard]] auto eval(const libataxx::Position &pos) -> int {
    const auto num_us = pos.us().count();
    const auto num_them = pos.them().count();
    return 100 * (num_us - num_them);
}
