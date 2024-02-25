
# ShiftTacToe
An implementation of Shift-Tac-Toe in C++ with a game tree to analyze optimal play. The main file specifies the starting moves and builds the tree beginning after those moves (from that configuration). This lets you build any part of the tree without using too many resources. This is a pretty technical implementation for efficiency and encodes a lot of data. Look up tables are provided below.

There is also an AI test file which lets you play against the optimal AI based on the tree you built. You always lose. :)

Please note that there is no error handling, so you definitely need to pay attention. To change options, you have to set the globals and recompile.


The actual Shift-Tac-Toe game from 1988: https://boardgamegeek.com/boardgame/23318/shift-tac-toe. To 3D print one: https://www.instructables.com/Shift-Tac-Toe/.
To play a software version: https://github.com/deftcoyote/ShiftTacToe.

## Segment Driver (segmentdriver.cpp)

### Running
To compile:
> g++ -o segmentdriver segmentdriver.cpp STTGameTree.h STT3.h 

To run:
> segmentdriver XYZ A B

 - XYZ is a path in the tree of starting moves and using that as the root (see Path Numbers in the game tree). If you want the root, use a dash "-"
 - A is the level to load. 7 will load PXYZ_7.pot if LOADPRUNE = true and XYZ_7.pot if LOADPRUNE = false. 0 will not load anything but build from scratch
 - B is the level to build to and save


for a new tree starting at path 11.
> ./segmentdriver 11 0 7

to then go to level 12
> ./segmentdriver 11 7 12

for a new tree from the root to level 7
> ./segmentdriver - 0 7

### Compile options
There are several global options at the top of the file (sorry they're not in a config file).

//load the pruned file rather than full tree (load PX_L.pot rather than X_L.pot where X and L are from the command line)
> bool LOADPRUNE = true;

//prompt for more levels (do you want to add levels or just load)
> bool ADDLEVELS = true;

//interactive prompt or not (not actually used, but was to give interactive prompts rather than command line options)
> bool LOCAL = true;

//output unpruned tree - stores as X_L.pot
> bool SAVE = false;

//run wld function and prune
> bool WLD = true;

//output pruned tree (only matters if WLD = true) - stores as PX_L.pot
> bool SAVEPRUNE = true;

### File Format
Always saves files as X_L.pot where X is the path in the tree it starts from, and L is the number of levels (FROM THE ROOT). Thus, 00_3.pot builds the tree from the root, but the first 2 levels only have the 0 options, so this file would only explore one new level. L > |X|. The level must always be larger than the length of the path.

The tree is stored as binary using a pre-order traversal (pot).

If SAVEPRUNE = true, then it will also prune unnecessary moves in the game tree and save that tree as PX_L.pot (just adds a 'P' to the front of the filename).



## AI Test (AItest.cpp)

This is to test the game tree, where the "AI" plays as first player using the optimal tree choices.

### Running
To compile:
> g++ -o AItest AItest.cpp STTGameTree.h STT3.h 

To run:
> ./AItest [filename] [starting path]

Without the parameters, it will ask for the pot file to load, and then for the path (see Path Numbers) of where to go down the tree to find the starting position.



## Game Tree (STTGameTree.h)
The game tree makes a few assumptions and does not build all possible moves. The only true draw condition for the game is when a shift occurs and both players win. However, since the game allows repetition, the tree would never end. The tree identifies a node as a leaf if 
- it is a win condition for either player
- it is a draw condition
- it is a draw through repetition (this configuration has already occurred as an ancestor of this node)

The state of a node is stored in the win character of the node.

The game tree is designed to be built up one move at a time (a level of the tree) through a breadth first search. Thus, the functions are to add levels.


### Node Object
For space efficiency, each node only stores the board configuration converted to a long integer (see Positions), a char win (see Win Character), and the children pointers to the 9 possible moves (see Path Numbers).

### Path Numbers
There are 9 possible moves at each configuration. Thus, the general tree has 9^n nodes at level n. A game can then be expressed as a character string of numbers. The numbers represent these moves. The piece added is based on the turn parity.

 - 0 \- add col 0
 - 1 \- add col 1
 - 2 \- add col 2
 - 3 \- shift row 0 left (top row)
 - 4 \- shift row 0 right
 - 5 \- shift row 1 left
 - 6 \- shift row 1 right
 - 7 \- shift row 2 left (bottom row)
 - 8 \- shift row 2 right

117 represents P1 placing a '1' in the middle column, P2 placing a '2' in the middle column, and then P1 shifting the bottom row left.

### Win Character
For the Win Lose Drow (WLD) tree, each node stores a char win, which tells you about the node. For space, we use a single char where it is one of these values.

 - X \- not set
 - 0 \- draw
 - 1 \- p1 wins leaf
 - 2 \- p2 wins leaf
 - 3 \- draw win leaf
 - 4 \- loop draw leaf
 - F \- first player win internal
 - S \- second player win internal
 - D \- draw internal (0 or 3)
 - L \- loop draw internal

## Shift Tac Toe class (STT3.h)

The class implementation of the board with a couple of helpful functions. This version stores the board as a string.

### Positions

Every position is stored as a 12 character string 123456789ABC where each numbered position is the location in the matrix (below). Every number is 0, 1, 2, where 0 is no piece, 1 is first player piece, and 2 is a 2nd player piece.

```
 col   0 1 2 
     | 1 2 3 | row 0
     | 4 5 6 | row 1
     | 7 8 9 | row 2
```

The ABC are numbers 0, 1, 2 for the shift of each row where A is row 0. A zero means that it is shifted all the way to the right. 

Example string 021 021 111 210

```
  - - | 0 2 1 | 
    - | 0 2 1 | - 
      | 1 1 2 | - -
```
Note that for the game tree, for storage efficiency, it is stored as an unsigned long int with a 5 before it in order to keep the length correct. So 021021111210 -> 5021021111210.

## doall script (doall.sh)
Just a small script to run the segment driver for a lot of different parts of the tree at once.
