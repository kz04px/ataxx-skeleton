#ifndef EVAL_HPP
#define EVAL_HPP

namespace libataxx {
class Position;
}  // namespace libataxx

// Return the evaluation of the position from the side to move's point of view
int eval(const libataxx::Position &pos);

#endif
