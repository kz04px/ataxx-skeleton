
### About
Skeleton code for an Ataxx engine written in C++. All the basic requirements for an engine are included, such as move generation and FEN parsing. A basic [minimax](https://en.wikipedia.org/wiki/Negamax) search and material only evaluation are also provided as examples. This project is intended to provide a solid foundation for others to build their own engines from.

---
### About the game
Ataxx is a two player, turn-based, perfect information board game created in 1988.<br/>
[Wikipedia article](https://en.wikipedia.org/wiki/Ataxx)

---
### Build instructions
```bash
git clone https://github.com/kz04px/ataxx-skeleton
cd ./ataxx-skeleton
make
```

---
### UAI protocol
UAI stands for "Universal Ataxx Interface" and is a slightly modified version of the Universal Chess Interface protocol.
The only differences are that instances of 'c' for 'chess' have been replaced by 'a' for 'ataxx', and that the [FEN](https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation) format has been altered to suit the game.

---
### FEN strings
"x" are player one's pieces. "o" are player two's pieces. Numbers represent the number of empty squares. Dashes represent blocked squares. The final character shows the current side to play.<br/>
Examples:
>x5o/7/7/7/7/7/o5x x</br>
x5o/7/2-1-2/7/2-1-2/7/o5x x

---
### Ataxx tools
Some potentially useful python scripts for Ataxx engine development, among other things, can be found here:
[Ataxx tools](https://github.com/kz04px/ataxx-tools)

---
### Portability
Should work on both Windows and Linux, however, this has not been thoroughly tested. The included makefile may run into issues where it can't create the folders 'bin' and 'obj' itself. To fix this, create both folders manually alongside the 'src' folder.

---
### Example UAI usage
>uai</br>
id name AtaxxEngine</br>
id author kz04px</br>
option name debug type check default false</br>
uaiok</br>
isready</br>
readyok</br>
go depth 5</br>
info score cs 100 depth 1 seldepth 1 time 0 nodes 14 nps 335530 pv f1</br>
info score cs 0 depth 2 seldepth 2 time 0 nodes 224 nps 1674065 pv f1 b1</br>
info score cs 100 depth 3 seldepth 3 time 1 nodes 4618 nps 4187051 pv f1 b1 e1</br>
info score cs 0 depth 4 seldepth 4 time 22 nodes 95540 nps 4232042 pv f1 b1 f2 c1</br>
info score cs 100 depth 5 seldepth 5 time 532 nodes 2452814 nps 4607308 pv f1 a2 e1 a3 d1</br>
bestmove f1</br>
quit
