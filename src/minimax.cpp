#include <limits>
#include <cassert>
#include "search.hpp"
#include "movegen.hpp"
#include "makemove.hpp"
#include "eval.hpp"

// Minimax algorithm (negamax)
int minimax(SearchStats &stats, SearchStack *stack, const Position &pos, const int depth)
{
    assert(stack);

    // Update seldepth stats
    stats.seldepth = std::max(stack->ply, stats.seldepth);

    // Return mate or draw scores if the game is over
    if(gameover(pos))
    {
        const int num_us = popcountll(pos.pieces[pos.turn]);
        const int num_them = popcountll(pos.pieces[!pos.turn]);

        if(num_us > num_them) {return MATE_SCORE - stack->ply;}
        else if(num_us < num_them) {return -MATE_SCORE + stack->ply;}
        else {return 0;}
    }

    // Make sure we stop searching
    if(depth == 0 || stack->ply >= MAX_DEPTH)
    {
        return eval(pos);
    }

    int best_score = std::numeric_limits<int>::min();

    // Move generation
    Move moves[MAX_MOVES];
    int num_moves = movegen(pos, moves);

    // Keeping track of the node count
    stats.nodes += num_moves;

    // Play every legal move and run negamax on the resulting position
    for(int i = 0; i < num_moves; ++i)
    {
        (stack+1)->pv.clear();

        Position npos = pos;
        makemove(npos, moves[i]);
        int score = -minimax(stats, stack+1, npos, depth-1);
        if(score > best_score)
        {
            // Update PV
            stack->pv.clear();
            stack->pv.push_back(moves[i]);
            stack->pv.insert(stack->pv.begin()+1, (stack+1)->pv.begin(), (stack+1)->pv.end());

            best_score = score;
        }
    }

    return best_score;
}
