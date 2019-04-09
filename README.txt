# CS 201 - Portfolio Project
Jonathan Blake - 11704031

Terminal Connect Four
https://github.com/jblake798/cs201.git

Demo Video
https://youtu.be/jDQsoDm77yY

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

----------
BOARD SIZE
----------

Although the game board can be set as any real size larger than 4x4, the
program will display a warning if the terminal window is too small to display
the board size chosen. For this reason, the option is given to resize the
game board after this warning would be displayed.

If a game board larger than can be displayed on the terminal is initiated,
the program will also display a warning on the ncurses window telling the
user that any display errors that are seen are being caused by the window
being too small.

Additionally, if the terminal window is resized during the game, the board will
automatically re-center itself and display a window size warning if the window
is resized to a size smaller than can accurately display the board.

------------------
STATE MACHINE FLOW
------------------

Basic state machine structure was originally based upon a sample ncurses
based worms game on http://www.paulgriffiths.net/program/c/curses.php.
Although this served as a loose base for the state machine, this state
machine has been expanded far beyond the simple application in the example
code. The following state diagram shows the program path prior to entering
the game state.

NOTE THAT THIS FLOWCHART DOES NOT DISPLAY ACCURATELY IN MARKDOWN

       +-----------+  
       | MAIN MENU |  quit  +--------------+  
+----->+   begin   +------->+ EXIT PROGRAM |  
|      |   quit    |        +--------------+  
|      +-----+-----+  
|            |  
|            | begin  
|            v  
|      +-----+---------+  
|      | CHOOSE NUMBER |  
|      |   OF HUMAN    |  
|      |   PLAYERS     |  
|      +-----+---------+  
|            |  
|            |  
|            v  
|      +-----+--------+  
|      | CHOOSE WHICH |  
|      | PLAYER WILL  |  
|      |   GO FIRST   |  
|      +-----+--------+  
|            |  
|            |  
|            v  
|      +-----+---------+  
|      | SPECIFY BOARD |  
|      | SIZE TO PLAY  +<------------+  
|      |    GAME ON    |             |  
|      +-----+---------+             |  
|            |                       |  
|            |                       |  
|            v                       |  
|      +-----+--------------+        |  
| exit | CONFIRM GAME START | resize |  
+<-----+       begin        +--------+  
^      |       resize       |  
|      |       exit         |  
|      +-----+--------------+  
|            |  
|            | begin  
|            v  
|      +-----+--------------+  
|      | GAME BOARD         |  
+------+ exits on tie,      |  
       | ENTER after a win, |  
       | or press of 'q'    |  
       +--------------------+

The game state machine swaps between two states: a player one state and
a player two state (either a second user or the computer). The game prints
the board, waits for the user to select a column to drop a token using
the game cursor, then decides what to do with the input before either
switching to the next player's turn or restarting the turn (in the case
of a full column).

Each turn state checks if the placed token creates a winning pattern and
checks if the board has been completely filled (a tie state).

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
