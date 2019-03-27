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
#include <unistd.h>
#include <signal.h>
#include <curses.h>

#include "helper.h"
#include "board.h"


// define state type

typedef enum { INITIALIZATION,
	       PROG_CONFIG,
	       GAME_CONFIG_START,
	       GAME_CONFIG_PLAYERNUM,
	       GAME_CONFIG_FIRSTPLAYER,
	       GAME_CONFIG_BOARDSIZE,
	       GAME_CONFIG_CONFIRMSTART,
	       GAME_WINDOW_INIT,
	       PLAYER_ONE_TURN,
	       PLAYER_TWO_TURN,
	       COMPUTER_TURN,
	       WIN_CONDITION,
	       CLOSE_GAME,
	       QUIT,
	       INVALID_STATE }
  STATE;


// global variables

WINDOW * gameWindow = NULL;
BoardNode * homeNode = NULL;
int oldcur;


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
      
      printf("\nCONNECT FOUR\n");
      
      printf("\nCurrent score:\n");
      
      // TODO UNCOMMENT SCORE INFORMATION
      // printf("Player v. Player: %d", pvpScore);
      // printf("Player v. Computer: %d", pvcompScore);

      
      // print list of options

      printf("\nMAIN MENU");
      printf("\nOptions: begin config quit\n");
      printf("Select option: ");
      
      state = INVALID_STATE;
  
      while ( state == INVALID_STATE ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	// check input and change state if valid
	if ( strcmp( input, "begin" ) == 0 )       state = GAME_CONFIG_START;
	else if ( strcmp( input, "config" ) == 0 ) state = PROG_CONFIG;
	else if ( strcmp( input, "quit" ) == 0 )   state = QUIT;
	else
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: begin config quit\n");
	    printf("Select option: ");
	  }
      }

      
      break;

      
    case PROG_CONFIG: /* PROGRAM CONFIGURATION STATE */

      // TODO DECIDE WHAT OPTIONS YOU WANT TO CONFIGURE

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

      int termRows, termCols;
      UpdateTermSize( &termRows, &termCols );

      // TODO MAKE SURE TERMINAL ALLOWED SIZE IS CORRECT
      
      if ( ( boardRows > ( termRows - 2 ) ) || ( ( ( 2 * boardCols ) - 1 ) > termCols ) ) {
	printf("\nWARNING: TERMINAL WINDOW IS CURRENTLY NOT LARGE ENOUGH TO PROPERLY DISPLAY BOARD");
	printf("\nCURRENT ALLOWED SIZE: %d x %d", ( termRows - 2 ), ( ( termCols / 2 ) + 1 ) );
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

      // create variable for finding user input
      int key;

      // create board with set dimensions
      homeNode = CreateBoard( boardRows, boardCols );

      // create variable for choosing column
      int cursor = 0;

      // TODO FIX READING ENTER KEY

      // print board and infographics
      PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
      mvwaddstr( gameWindow, 0, ( termCols - 28 ), "OPTIONS: 'q'-> QUIT 'e'->ENTER");
      refresh();

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
      PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
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
	  PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  refresh();
	  break;

	case KEY_LEFT:
	case 'a':
	case 'A':
	  if ( cursor > 0 ) --cursor;
	  PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
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
	
      } while ( key != 'e' );

      // catch quit statement before moving on
      if ( state == CLOSE_GAME ) break;

      // place token in desired column, catch error if column full
      if ( DropToken( homeNode, cursor, ONE ) == 0 ) {
	
	// print error
	if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	mvwaddstr( gameWindow, ( termRows / 2 ), ( ( termCols / 2 ) - 5 ), "COLUMN FULL" );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	refresh();

	// sleep for 3 seconds
	sleep(3);

	// clear error
	mvwaddstr( gameWindow, ( termRows / 2 ), ( ( termCols / 2 ) - 5 ), "           " );
	refresh();
	
	// send back to start of turn
	state = PLAYER_ONE_TURN;
	break;
	
      }

      // TODO CHECK FOR WIN

      // go to next turn
      if ( players == 1 ) state = COMPUTER_TURN;
      else if ( players == 2 ) state = PLAYER_TWO_TURN;
      else state = INVALID_STATE;

      break;

    case PLAYER_TWO_TURN: /* PLAYER TWO'S TURN */

      // set cursor to start
      cursor = 0;
      PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
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
	  PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  refresh();
	  break;

	case KEY_LEFT:
	case 'a':
	case 'A':
	  if ( cursor > 0 ) --cursor;
	  PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols, cursor );
	  refresh();
	  break;

	case KEY_DC:
	case 'q':
	case 'Q':
	  state = CLOSE_GAME;
	  key = 'e';
	  break;

	case KEY_ENTER:
	case 'e':
	case 'E':
	  key = 'e';
	  break;
	  
	}
	
      } while ( key != 'e' );

      // catch quit statement before moving on
      if ( state == CLOSE_GAME ) break;

      // place token in desired column, catch error if column full
      if ( DropToken( homeNode, cursor, TWO ) == 0 ) {
	
	// print error
	if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	mvwaddstr( gameWindow, ( termRows / 2 ), ( ( termCols / 2 ) - 5 ), "COLUMN FULL" );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	refresh();

	// sleep for 3 seconds
	sleep(3);

	// clear error
	mvwaddstr( gameWindow, ( termRows / 2 ), ( ( termCols / 2 ) - 5 ), "           " );
	refresh();
	
	// send back to start of turn
	state = PLAYER_TWO_TURN;
	break;
	
      }

      // TODO CHECK FOR WIN

      // go to next turn
      state = PLAYER_ONE_TURN;

      break;
      
    case COMPUTER_TURN: /* COMPUTER'S TURN */

      // print player turn
      if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
      mvwaddstr( gameWindow, 0, 0, "COMPUTER TURN");
      if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );

      // TODO AI MOVE

      // TODO CHECK FOR WIN

      // go to next turn
      state = PLAYER_ONE_TURN;

      break;


    case WIN_CONDITION: /* WINNING STATE */

      // TODO WIN STATE

      break;


    case CLOSE_GAME: /* CLOSE GAME BOARD AND RETURN TO INIT */

      endwin();
      FreeBoard(homeNode);
      homeNode = NULL;

      printf("\nGame was closed by user, going to main menu...\n");

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

  struct sigaction sa;
  sa.sa_handler = handler;
  sa.sa_flags   = 0;
  sigemptyset(&sa.sa_mask);

  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT,  &sa, NULL);

  // Ignore SIGTSTP

  sa.sa_handler = SIG_IGN;
  sigaction(SIGTSTP, &sa, NULL);

}
