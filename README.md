# About
Skeleton code for an Ataxx engine written in C++. All the basic requirements for an engine are included. A basic [minimax](https://en.wikipedia.org/wiki/Negamax) search and material only evaluation are also provided as examples. This project is intended to provide a solid foundation for others to build their own engines from.

---

### About the game
Ataxx is a two player, turn-based, perfect information board game created in 1988.<br/>
[Wikipedia article](https://en.wikipedia.org/wiki/Ataxx)

---

### Build instructions
```bash
git clone --recurse-submodules https://github.com/kz04px/ataxx-skeleton
follow libataxx build instructions
mkdir ./ataxx-skeleton/build
cd ./ataxx-skeleton/build
cmake ..
make
```

---

### UAI protocol
UAI stands for "Universal Ataxx Interface" and is a slightly modified version of the Universal Chess Interface protocol.
The only differences are that instances of 'c' for 'chess' have been replaced by 'a' for 'ataxx', and that the [FEN](https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation) format has been altered to suit the game.

---

### FEN strings
Based on [FEN strings](https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation) from chess.<br/>
Player one is black and represented by 'x'<br/>
Player two is white and represented by 'o'<br/>
Gaps/Blockers are represented by '-'<br/>
Examples:
```
x5o/7/7/7/7/7/o5x x 0 1
x-----o
-------
-------
-------
-------
-------
o-----x
```
```
xx4o/5o1/2-1-2/7/2-1-2/7/o5x x 0 2
xx----o
-----o-
-- - --
-------
-- - --
-------
o-----x
```

---

### Example UAI usage
```
uai
id name AtaxxEngine
id author kz04px
option name debug type check default false
uaiok
isready
readyok
uainewgame
position startpos
go depth 5
info score cp 100 depth 1 seldepth 1 time 0 nodes 14 nps 335530 pv f1
info score cp 0 depth 2 seldepth 2 time 0 nodes 224 nps 1674065 pv f1 b1
info score cp 100 depth 3 seldepth 3 time 1 nodes 4618 nps 4187051 pv f1 b1 e1
info score cp 0 depth 4 seldepth 4 time 22 nodes 95540 nps 4232042 pv f1 b1 f2 c1
info score cp 100 depth 5 seldepth 5 time 532 nodes 2452814 nps 4607308 pv f1 a2 e1 a3 d1
bestmove f1
quit
```

---

### Thanks
- [doctest](https://github.com/doctest/doctest) - The fastest feature-rich C++11/14/17/20 single-header testing framework 
