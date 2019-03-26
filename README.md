# cs201 portfolio


initialization:
	1. game start
	2. options
	3. exit

game start:
	1. decide if playing
	   a. player v player
	   b. player v computer
	      i. player first
	      ii. computer first
	2. decide request board size
	   a. column
	   b. row
	   c. standard is 7x6
	3. create board
	4. begin game

options:
	1. board printing options
	2. display next best move?

main loop:
	1. states
	   a. NOTE: initial state set at start. state flipped at end of each turn.
	   b. PLAYER
	      i. ask position to insert piece
	      ii. insert piece on board
	      iii. check win condition
	   c. COMPUTER
	      i. search for best move
	      ii. insert piece on board
	      iii. check win condition
	2. loop
	   a. main loops over switch case
	   b. state switched to next proper state before breaking from case
	3. win condition
	   a. satisfies wrapping loop
	   b. when met, confetti
	   c. if max number of moves have transpired, end in draw (no confetti)
	   d. return to initialization function

libraries:
	1. board.h (contains board)
	   a. object is linked list graph of board locations
	   b. functions
	      - create board
	      - check if playable
	      - insert piece
	      - check if move will win
	      - print board

