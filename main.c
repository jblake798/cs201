/*
 *  CS 201 - Portfolio Project
 *  Jonathan Blake - 11704031
 *  Terminal Connect Four
 *
 *  ======
 *
 *  MAIN.C
 *  primary state machine loop
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <curses.h>

#include "helper.h"
#include "board.h"


// define state type

typedef enum { INITIALIZATION,
	       GAME_CONFIG_START,
	       GAME_CONFIG_PLAYERNUM,
	       GAME_CONFIG_FIRSTPLAYER,
	       GAME_CONFIG_BOARDSIZE,
	       GAME_CONFIG_CONFIRMSTART,
	       GAME_WINDOW_INIT,
	       PLAYER_ONE_TURN,
	       PLAYER_TWO_TURN,
	       COMPUTER_TURN,
	       DRAW_CONDITION,
	       WIN_CONDITION,
	       CLOSE_GAME,
	       QUIT,
	       INVALID_STATE }
  STATE;


// global variables

WINDOW * gameWindow = NULL;
int oldcur;

int pvpGames = 0;
int pvpP1Wins = 0;
int pvpP2Wins = 0;

int pvcGames = 0;
int pvcP1Wins = 0;
int pvcAIWins = 0;


// function declarations

void setupSignals(void);


/*  Main  */

int main (void)
{
  
  setupSignals();

  // set random seed
  srand(time(NULL));

  STATE state = INITIALIZATION;
  
  // main loop
  
  while ( state != QUIT ) {
    
    switch ( state ) {
      
    case INITIALIZATION: /* INITIALIZATION STATE */

      // print initial greeting
      
      printf("\n\nCONNECT FOUR\n\n");
      
      printf("\nCurrent scores:\n");
      
      printf("\nPlayer v. Player games played: %d\n", pvpGames);
      printf("\tPlayer 1 wins: %d\n", pvpP1Wins);
      printf("\tPlayer 2 wins: %d\n", pvpP2Wins);
      
      printf("\nPlayer v. Computer games played: %d\n", pvcGames);
      printf("\tPlayer 1 wins: %d\n", pvcP1Wins);
      printf("\tComputer wins: %d\n", pvcAIWins);
      
      
      // print list of options

      printf("\nMAIN MENU");
      printf("\nOptions: begin quit\n");
      printf("Select option: ");
      
      state = INVALID_STATE;
  
      while ( state == INVALID_STATE ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	// check input and change state if valid
	if ( strcmp( input, "begin" ) == 0 )       state = GAME_CONFIG_START;
	else if ( strcmp( input, "quit" ) == 0 )   state = QUIT;
	else
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: begin quit\n");
	    printf("Select option: ");
	  }
      }

      
      break;
      

    case GAME_CONFIG_START: /* START OF GAME CONFIG STATE */

      printf("\nEntering game configuration...\n");

      
    case GAME_CONFIG_PLAYERNUM: /* GAME CONFIG STATE PLAYER NUMBER CHOICE */

      printf("\nHow many human players are there?");
      printf("\nOptions: one two\n");
      printf("Choice: ");
      
      int players = 0;
      
      while ( players == 0 ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	// check input and set players if valid
	if ( strcmp( input, "one" ) == 0 )      players = 1;
	else if ( strcmp( input, "two" ) == 0 ) players = 2;
	else
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: one two\n");
	    printf("Select number of players: ");
	  }
      }

      // informative output
      if ( players == 1 ) printf("\nYou are player one!\nThe computer is player two!\n");
      else if ( players == 2 ) printf("\nPlayer one and player two will take turns with the keyboard!\nHonor system!\n");

      
    case GAME_CONFIG_FIRSTPLAYER: /* GAME CONFIG STATE FIRST PLAYER CHOICE */
      
      printf("\nWho will go first?");
      printf("\nOptions: one two random\n");
      printf("First player: ");

      PLAYER firstTurn = INVALID_PLAYER;
      
      while ( firstTurn == INVALID_PLAYER ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	// check input and set player appropriately
	if ( strcmp( input, "one" ) == 0 ) firstTurn = ONE;
	else if ( strcmp( input, "two" ) == 0 ) firstTurn = TWO;
	else if ( strcmp( input, "random" ) == 0 ) {
	  if ( rand() % 2 ) firstTurn = ONE;
	  else firstTurn = TWO;
	} else
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: one two random\n");
	    printf("Select who goes first: ");
	  }
      }

      // informative output
      if ( firstTurn == ONE ) printf("\nPlayer one will go first!\n");
      else if ( firstTurn == TWO ) printf("\nPlayer two will go first!\n");

      
    case GAME_CONFIG_BOARDSIZE: /* GAME CONFIG STATE BOARD SIZE CHOICE */

      printf("\nEnter size of board... (The standard board size is 6x7)");

      // get number of rows
      printf("\nRows: ");
      int boardRows;
      int status = scanf("%d", &boardRows);
      while ( ( boardRows < 4 ) || ( status == EOF ) || ( status == 0 ) ) {
	printf("\nPlease enter a valid integer larger than four...");
	printf("\nRows: ");
	while ( fgetc(stdin) != '\n' );
	status = scanf("%d", &boardRows);
      }

      // get number of columns
      printf("Columns: ");
      int boardCols;
      status = scanf("%d", &boardCols);
      while ( ( boardCols < 4 ) || ( status == EOF ) || ( status == 0) ) {
	printf("\nPlease enter a valid integer larger than four...");
	printf("\nColumns: ");
	while ( fgetc(stdin) != '\n' );
	status = scanf("%d", &boardCols);
      }

      // informative output
      printf("\nBoard will be %d x %d\n", boardRows, boardCols);

      // terminal size tracking variables
      int termRows, termCols = 0;
      int termRowsPrev, termColsPrev = 0;
      UpdateTermSize( &termRows, &termCols );

      // warn if terminal is too small
      if ( ( boardRows > ( termRows - 3 ) ) || ( boardCols > ( ceil( (double) termCols / 2 ) ) ) ) {
	printf("\nWARNING: TERMINAL WINDOW IS CURRENTLY NOT LARGE ENOUGH TO PROPERLY DISPLAY BOARD");
	printf("\nCURRENT ALLOWED SIZE: %d x %d", ( termRows - 3 ), (int) ( ceil( (double) termCols / 2 ) ) );
	printf("\nVISUAL ERRORS MAY OCCUR\n");
      }


    case GAME_CONFIG_CONFIRMSTART: /* GAME CONFIG STATE CONFIRM START */

      printf("\nWould you like to BEGIN the game, EXIT to main menu, or RESIZE the board?");
      printf("\nOptions: begin menu resize\n");
      printf("Answer: ");

      state = INVALID_STATE;
      
      while ( state == INVALID_STATE ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	if ( strcmp( input, "begin" ) == 0 ) {
	  state = GAME_WINDOW_INIT;
	  
	} else if ( strcmp( input, "menu" ) == 0 ) {
	  state = INITIALIZATION;
	  printf("\nGoing to main menu...\n");

	} else if ( strcmp( input, "resize" ) == 0 ) {
	  state = GAME_CONFIG_BOARDSIZE;
	  printf("\nGoing back to board size decision...\n");
	  
	} else {
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: begin menu resize\n");
	    printf("Answer: ");
	  }	  
	}
      }
      
      break;


    case GAME_WINDOW_INIT: /* GAME WINDOW START */

      // initialize ncurses
      if ( gameWindow == NULL ) {
	if ( ( gameWindow = initscr() ) == NULL ) {
	  perror("error initializing ncurses");
	  exit(EXIT_FAILURE);
	}
      } else {
	werase(gameWindow);
	refresh();
      }

      // save previous state
      oldcur = curs_set(0);

      // initialize window settings
      noecho();
      keypad( gameWindow, TRUE );
      if ( has_colors() == TRUE ) {
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
      } else printf("\nNOTE :: Your terminal does not support color\n");

      // create board with set dimensions
      int **board = (int **) malloc( boardCols * sizeof(int*) );
      for ( int i = 0 ; i < boardCols ; i++ )
	board[i] = (int *) malloc( boardRows * sizeof(int) );
      for ( int i = 0 ; i < boardCols ; i++ )
	for ( int j = 0 ; j < boardRows ; j++ )
	  board[i][j] = 0;

      // create array that tracks how much each column is full
      int *height = (int *) malloc( boardCols * sizeof(int) );
      for ( int i = 0 ; i < boardCols ; i++ )
	height[i] = 0;
      
      // create variable for choosing column
      int cursor = 0;

      // create variable for finding user input
      int key;

      // print board and infographics
      PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      mvwaddstr( gameWindow, 0, ( termCols - 31 ), "OPTIONS: 'q'-> QUIT 'e'->ENTER");
      refresh();

      // print warning if window too small
      if ( ( boardRows > ( termRows - 3 ) ) || ( boardCols > ( ceil( (double) termCols / 2 ) ) ) )
	mvwaddstr( gameWindow, 1, ( termCols - 17 ), "WINDOW TOO SMALL");

      // create variables for keeping track of game information
      PLAYER winner = NONE;
      int move = 0;
      int movesMax = boardCols * boardRows;


      // go to proper first turn state
      if ( firstTurn == ONE ) state = PLAYER_ONE_TURN;
      
      else if ( firstTurn == TWO ) {
	
	if ( players == 1 ) state = COMPUTER_TURN;
	else if ( players == 2 ) state = PLAYER_TWO_TURN;
	else state = INVALID_STATE;
	
      }

      else state = INVALID_STATE;

      break;

      
    case PLAYER_ONE_TURN: /* PLAYER ONE'S TURN */

      // set cursor to start
      cursor = 0;
      PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      mvwaddstr( gameWindow, 0, ( termCols - 31 ), "OPTIONS: 'q'-> QUIT 'e'->ENTER");
      refresh();

      // print player turn
      if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
      mvwaddstr( gameWindow, 0, 0, "PLAYER ONE TURN");
      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
      refresh();

      // loop for user input
      do {

	key = getch();
	
	switch ( key ) {

	case KEY_RIGHT:
	case 'd':
	case 'D':
	  if ( cursor < ( boardCols - 1 ) ) ++cursor;
	  PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  refresh();
	  break;

	case KEY_LEFT:
	case 'a':
	case 'A':
	  if ( cursor > 0 ) --cursor;
	  PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  refresh();
	  break;

	case KEY_DC:
	case 'q':
	case 'Q':
	  state = CLOSE_GAME;
	  key = 'e';
	  break;

	case KEY_ENTER:
	case '\n':
	case 'e':
	case 'E':
	  key = 'e';
	  break;
	  	  
	}

	// resize window if terminal window changed
	termRowsPrev = termRows;
	termColsPrev = termRows;
	UpdateTermSize( &termRows, &termCols );

	// reprint if necessary
	if ( ( termRows != termRowsPrev ) || ( termCols != termColsPrev ) ) {
	  
	  werase( gameWindow );
	  
	  PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  mvwaddstr( gameWindow, 0, ( termCols - 31 ), "OPTIONS: 'q'-> QUIT 'e'->ENTER");
	  
	  if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	  mvwaddstr( gameWindow, 0, 0, "PLAYER ONE TURN");
	  if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	  
	  refresh();
	}

	// print warning if window too small
	if ( ( boardRows > ( termRows - 3 ) ) || ( boardCols > ( ceil( (double) termCols / 2 ) ) ) )
	  mvwaddstr( gameWindow, 1, ( termCols - 17 ), "WINDOW TOO SMALL");
	
      } while ( key != 'e' );

      
      // catch quit statement before moving on
      if ( state == CLOSE_GAME ) break;

      // check if winning move
      if ( IsWinningMove( board, height, boardRows, boardCols, cursor, 1 ) == 1 ) {
	board[cursor][height[cursor]] = 1;
	PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	refresh();
	winner = ONE;
	state = WIN_CONDITION;
	break;
      }

      // place token in desired column, catch error if column full
      if ( CanPlay( height, boardRows, cursor ) ) {

	board[cursor][height[cursor]] = 1;
	height[cursor]++;
	move++;

      } else {	
	
	// print error
	if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "COLUMN FULL" );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	refresh();

	// sleep for 2 seconds
	sleep(2);

	// clear error
	mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "           " );
	refresh();
	
	// send back to start of turn
	state = PLAYER_ONE_TURN;
	break;
	
      }

      // print board
      PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      refresh();

      // check if draw
      if ( move == movesMax ) {
	state = DRAW_CONDITION;
	break;
      }

      // go to next turn
      if ( players == 1 ) state = COMPUTER_TURN;
      else if ( players == 2 ) state = PLAYER_TWO_TURN;
      else state = INVALID_STATE;

      break;

    case PLAYER_TWO_TURN: /* PLAYER TWO'S TURN */

      // set cursor to start
      cursor = 0;
      PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      mvwaddstr( gameWindow, 0, ( termCols - 31 ), "OPTIONS: 'q'-> QUIT 'e'->ENTER");
      refresh();

      // print player turn
      if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
      mvwaddstr( gameWindow, 0, 0, "PLAYER TWO TURN");
      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );

      // loop for user input
      do {

	key = getch();
	
	switch ( key ) {

	case KEY_RIGHT:
	case 'd':
	case 'D':
	  if ( cursor < ( boardCols - 1 ) ) ++cursor;
	  PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  refresh();
	  break;

	case KEY_LEFT:
	case 'a':
	case 'A':
	  if ( cursor > 0 ) --cursor;
	  PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  refresh();
	  break;

	case KEY_DC:
	case 'q':
	case 'Q':
	  state = CLOSE_GAME;
	  key = 'e';
	  break;

	case 10:
	case 'e':
	case 'E':
	  key = 'e';
	  break;
	  
	}

	// resize window if terminal window changed
	termRowsPrev = termRows;
	termColsPrev = termRows;
	UpdateTermSize( &termRows, &termCols );

	// reprint if necessary
	if ( ( termRows != termRowsPrev ) || ( termCols != termColsPrev ) ) {
	  
	  werase( gameWindow );
	  
	  PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  mvwaddstr( gameWindow, 0, ( termCols - 31 ), "OPTIONS: 'q'-> QUIT 'e'->ENTER");
	  
	  if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
	  mvwaddstr( gameWindow, 0, 0, "PLAYER TWO TURN");
	  if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );
	  
	  refresh();
	}

	// print warning if window too small
	if ( ( boardRows > ( termRows - 3 ) ) || ( boardCols > ( ceil( (double) termCols / 2 ) ) ) )
	  mvwaddstr( gameWindow, 1, ( termCols - 17 ), "WINDOW TOO SMALL");
	
      } while ( key != 'e' );

      // catch quit statement before moving on
      if ( state == CLOSE_GAME ) break;

      // check if winning move
      if ( IsWinningMove( board, height, boardRows, boardCols, cursor, 2 ) == 1 ) {
	board[cursor][height[cursor]] = 2;
	PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	refresh();
	winner = TWO;
	state = WIN_CONDITION;
	break;
      }

      // place token in desired column, catch error if column full
      if ( CanPlay( height, boardRows, cursor ) ) {

	board[cursor][height[cursor]] = 2;
	height[cursor]++;
	move++;

      } else {	
	
	// print error
	if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "COLUMN FULL" );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	refresh();

	// sleep for 2 seconds
	sleep(2);

	// clear error
	mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "           " );
	refresh();
	
	// send back to start of turn
	state = PLAYER_TWO_TURN;
	break;
	
      }

      // print board
      PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      refresh();

      // increment move counter and check if draw
      if ( move == movesMax ) {
	state = DRAW_CONDITION;
	break;
      }

      // go to next turn
      state = PLAYER_ONE_TURN;

      break;
      
    case COMPUTER_TURN: /* COMPUTER'S TURN */

      // set cursor to start
      cursor = 0;
      PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      mvwaddstr( gameWindow, 0, ( termCols - 31 ), "OPTIONS: 'q'-> QUIT 'e'->ENTER");
      
      refresh();

      // print player turn
      if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
      mvwaddstr( gameWindow, 0, 0, "COMPUTER TURN  ");
      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );

      refresh();

      int decision = AIDecision( board, height, boardRows, boardCols, move );
      
      // check if winning move ; break if so
      if ( IsWinningMove( board, height, boardRows, boardCols, decision, 2 ) == 1 ) {
	board[decision][height[decision]] = 2;
	PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, decision );
	refresh();
	winner = TWO;
	state = WIN_CONDITION;
	break;
      }

      // play decision
      board[decision][height[decision]] = 2;
      height[decision]++;
      move++;

      // print board
      PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      refresh();

      // increment move counter and check if draw
      if ( move == movesMax ) {
	state = DRAW_CONDITION;
	break;
      }

      // go to next turn
      state = PLAYER_ONE_TURN;

      break;
      

    case DRAW_CONDITION: /* IT'S A TIE! */

      mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "IT'S A TIE" );
      mvwaddstr( gameWindow, 0, ( termCols - 31 ), "     MAIN MENU IN 2 SECONDS...");
      refresh();

      // wait for two seconds
      sleep(2);
      
      state = CLOSE_GAME;

      break;

    case WIN_CONDITION: /* WINNING STATE */

      // go to proper first turn state
      if ( winner == ONE ) {

	// player one is winner
	if ( players == 1 ) {
	  
	  ++pvcGames;
	  ++pvcP1Wins;
	  
	  if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	  mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "PLAYER ONE WINS" );
	  if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	  
	} else if ( players == 2 ) {
	  
	  ++pvpGames;
	  ++pvpP1Wins;

	  if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	  mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "PLAYER ONE WINS" );
	  if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );

	}
	
      } else if ( winner == TWO ) {
	
	if ( players == 1 ) {

	  // computer is winner
	  ++pvcGames;
	  ++pvcAIWins;

	  if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
	  mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "COMPUTER WINS" );
	  if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );
	
	} else if ( players == 2 ) {

	  // player two is winner
	  ++pvpGames;
	  ++pvpP2Wins;

	  if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
	  mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "PLAYER TWO WINS" );
	  if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );
	  
	} else state = INVALID_STATE;
	
      } else state = INVALID_STATE;

      refresh();

      if ( state == INVALID_STATE ) break; // you should never get here. something went wrong.

      // print exit info
      mvwaddstr( gameWindow, 0, ( termCols - 31 ), " PRESS ENTER TO RETURN TO MENU");
      refresh();

      // loop for user input
      do {

	key = getch();
	
	switch ( key ) {
	  
	case KEY_ENTER:
	case '\n':
	case 'e':
	case 'E':
	  key = 'e';
	  break;
	  	  
	}

	// resize window if terminal window changed
	termRowsPrev = termRows;
	termColsPrev = termRows;
	UpdateTermSize( &termRows, &termCols );

	// reprint if necessary
	if ( ( termRows != termRowsPrev ) || ( termCols != termColsPrev ) ) {
	  
	  werase( gameWindow );
	  
	  PrintBoard( board, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  mvwaddstr( gameWindow, 0, ( termCols - 31 ), " PRESS ENTER TO RETURN TO MENU");

	  if ( winner == ONE ) {
	    
	    if ( players == 1 ) {
	      
	      if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	      mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "PLAYER ONE WINS" );
	      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	      
	    } else if ( players == 2 ) {

	      if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	      mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "PLAYER ONE WINS" );
	      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );

	    }

	  } else if ( winner == TWO ) {

	    if ( players == 1 ) {
	      
	      if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
	      mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "COMPUTER WINS" );
	      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );
	      
	    } else if ( players == 2 ) {

	      if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
	      mvwaddstr( gameWindow, ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 ), ( ( termCols / 2 ) - ( boardCols ) ), "PLAYER TWO WINS" );
	      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );

	    }
	  
	    refresh();
	  
	  }

	}

	// print warning if window too small
	if ( ( boardRows > ( termRows - 3 ) ) || ( boardCols > ( ceil( (double) termCols / 2 ) ) ) ) {
	  mvwaddstr( gameWindow, 1, ( termCols - 17 ), "WINDOW TOO SMALL");
	  refresh();
	}

	
	
      } while ( key != 'e' );


      // reset
      winner = NONE;

    case CLOSE_GAME: /* CLOSE GAME BOARD AND RETURN TO INIT */

      endwin();
      
      // free function
      for ( int i = 0 ; i < boardCols ; i++ )
	free( board[i] );
      free( board );
      free( height );

      printf("\nGoing to main menu...\n");

      state = INITIALIZATION;

      break;
      

    case INVALID_STATE: /* INVALID STATE */

      Error_Quit("\n\nENTERED INVALID STATE\n\n");

      break;
      
    case QUIT: /* QUIT */

      Quit(0);

      break;

    }

  }

  // code will never get here

}



void setupSignals (void)
{
  
  // set up signal handlers for exit interrupts

  signal(SIGTERM, handler);
  signal(SIGINT,  handler);
  signal(SIGTSTP, handler);

}
