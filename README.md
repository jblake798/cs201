# CS 201 - Portfolio Project
Jonathan Blake - 11704031

Terminal Connect Four
https://github.com/jblake798/cs201.git

-----
ABOUT
-----

This program hosts a connect four game to be played by one or two users.
Number of players, which player goes first, and the size of the board to
be played on are all chosen by the user prior to the start of each game.
When a game completes, the program returns to the main menu and updates
a scoreboard with the number of wins by each player in each gamemode.

-------------
SINGLE PLAYER
-------------

If a single person is playing, they will play against a computer opponent
that recursively analyizes a predictive graph of the game to find an
optimal move to play.

The recursive function utilized by the computer to progressively search
the predictive graph of the boards potential futures (the negamax function)
was based upon the recursive function created in the tutorial found on
http://blog.gamesolver.org/. This function recursively explores all
potential game paths reachable from the current game state. The solver
algorithm itself is capable of choosing the perfect move to win (or tie)
every game.

----------
TWO PLAYER
----------

If two people are playing, they play against eachother, taking turns at
the keyboard to drop their tokens in to the board. The honor system
applies to who drops the token on the board, of course.

--------
CONTROLS
--------

To move the cursor on the gameboard, you can use the LEFT and RIGHT
arrow keys, or 'A' and 'D' on the keyboard. To drop a token on the board,
you can use either the ENTER key or 'E'. Having two options for keyboard
input allows for two players to have their hands ready on the keyboard
for input at the same time during two player mode.

------------------
STATE MACHINE FLOW
------------------

'''flow
st=>start: Start
op1=>operation: Main Menu
op2=>operation: Number of players
op3=>operation:	Who goes first
op4=>operation: Board size
op5=>operation: Confirm Game Start
op6=>operation: Game Board
cond1=>condition: begin or quit
cond2=>condition: begin or menu or resize
e=>end: Terminate

st->op1->cond1
cond1(begin)->op2
cond1(quit)->e
op2->op3->op4->op5->cond2
cond2(begin)->op2
cond2(menu)->op1
cond2(resize)->op4

'''

------------------
WIN CHECK FUNCTION
------------------

Rather than check for the entire board for a win at the conclusion of
each turn for a win at every position, it was instead determined that
it would be more efficient to check if the token placed during a turn
created a winning condition. This simplified that state check significantly.
The specific algorithm used for this function was based upon the win
condition check used in the http://blog.gamesolver.org/ project. This
algorithm used a concise number of for loops in a novel way to check
for both horizontal and diagonal wins in the same loop.

------------------------------
A NOTE ON DATA STRUCTURE USAGE
------------------------------

Originially, I wrote this project using an adjacency graph to store
board information. This was accomplished using a struct that contained
eight pointers to adjacent nodes. Although this made graph navigation
more intuitive while programming, the data structure itself was very
large and took more time to navigate. Although I had written the entire
program using this data structure, I had to rewrite it to implement the
computer solving routine after discovering that stack overflow would occur
midway through the recursive solver function's solution.