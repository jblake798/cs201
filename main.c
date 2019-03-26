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
#include <signal.h>
#include <curses.h>

#include "helper.h"
#include "board.h"


// defining types

typedef enum { INITIALIZATION, PROG_CONFIG, GAME_CONFIG, GAME_WINDOW, QUIT, INVALID_STATE } STATE;


// global variables

WINDOW * gameWindow;
BoardNode * homeNode;
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
      
    case INITIALIZATION:

      // print initial greeting
      
      printf("\nCONNECT FOUR\n");
      
      printf("\nCurrent score:\n");
      
      // TODO UNCOMMENT SCORE INFORMATION
      // printf("Player v. Player: %d", pvpScore);
      // printf("Player v. Computer: %d", pvcompScore);

      
      // print list of options
      
      printf("\nOptions: begin config quit\n");
      printf("Select option: ");
      
      state = INVALID_STATE;
  
      while ( state == INVALID_STATE ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';
 
	if ( strcmp( input, "begin" ) == 0 )       state = GAME_CONFIG;
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

      
    case PROG_CONFIG:

      // TODO DECIDE WHAT OPTIONS YOU WANT TO CONFIGURE

      break;


    case GAME_CONFIG:

      printf("\nEntering game configuration...\n");

      
      // number of players

      printf("\nOptions: one two\n");
      printf("Select number of players: ");

      int players = 0;
      
      while ( players == 0 ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	if ( strcmp( input, "one" ) == 0 )      players = 1;
	else if ( strcmp( input, "two" ) == 0 ) players = 2;
	else
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: one two\n");
	    printf("Select number of players: ");
	  }
      }

      if ( players == 1 ) printf("\nYou are player one!\nThe computer is player two!\n");
      else if ( players == 2 ) printf("\nPlayer one and player two will take turns with the keyboard!\nHonor system!\n");


      // who goes first?

      printf("\nOptions: one two random\n");
      printf("Select which player goes first: ");

      PLAYER turn = INVALID_PLAYER;
      
      while ( turn == INVALID_PLAYER ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	if ( strcmp( input, "one" ) == 0 ) turn = ONE;
	else if ( strcmp( input, "two" ) == 0 ) turn = TWO;
	else if ( strcmp( input, "random" ) == 0 ) {
	  if ( rand() % 2 ) turn = ONE;
	  else turn = TWO;
	} else
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: one two random\n");
	    printf("Select who goes first: ");
	  }
      }

      if ( turn == ONE ) printf("\nPlayer one will go first!\n");
      else if ( turn == TWO ) printf("\nPlayer two will go first!\n");


      // size of board

      printf("\nEnter size of board... (The standard board size is 6x7)");
      
      printf("\nRows: ");
      int boardRows;
      int status = scanf("%d", &boardRows);
      while ( ( boardRows < 4 ) || ( status == EOF ) || ( status == 0 ) ) {
	printf("\nPlease enter a valid integer larger than four...");
	printf("\nRows: ");
	while ( fgetc(stdin) != '\n' );
	status = scanf("%d", &boardRows);
      }

      printf("Columns: ");
      int boardCols;
      status = scanf("%d", &boardCols);
      while ( ( boardCols < 4 ) || ( status == EOF ) || ( status == 0) ) {
	printf("\nPlease enter a valid integer larger than four...");
	printf("\nColumns: ");
	while ( fgetc(stdin) != '\n' );
	status = scanf("%d", &boardCols);
      }

      printf("\nBoard will be %d x %d\n", boardRows, boardCols);

      int termRows, termCols;
      UpdateTermSize( &termRows, &termCols );
      
      if ( ( boardRows > termRows ) || ( boardCols > termCols ) ) {
	printf("\nWARNING: TERMINAL WINDOW IS NOT LARGE ENOUGH TO PROPERLY DISPLAY BOARD");
	printf("\nTERMINAL SIZE: %d x %d", termRows, termCols);
	printf("\nVISUAL ERRORS MAY OCCUR\n");
      }


      // confirm game start

      printf("\nWould you like to begin the game?");
      printf("\nOptions: yes no\n");
      printf("Answer: ");
      
      while ( state == GAME_CONFIG ) {
	// read user input
	char input[32];
	fgets( input, 31, stdin );
	input[ strlen(input) - 1 ] = '\0';

	if ( strcmp( input, "yes" ) == 0 ) {
	  state = GAME_WINDOW;
	  printf("\nHEEEEEEREEEEE WEEEEEEE GOOOO!\n");
	} else if ( strcmp( input, "no" ) == 0 ) {
	  state = INITIALIZATION;
	  printf("\nGoing back to initial menu...\n");
	} else
	  if ( strlen(input) > 0 ) {
	    printf("\nINVALID OPTION\n");
	    printf("\nOptions: yes no\n");
	    printf("Answer: ");
	  }
      }

      
      break;


    case GAME_WINDOW:
      
      // initialize ncurses and create game window

      if ( ( gameWindow = initscr() ) == NULL ) {
	perror("error initializing ncurses");
	exit(EXIT_FAILURE);
      }

      noecho();
      keypad( gameWindow, TRUE );
      oldcur = curs_set(0);

      // create board with set dimensions
      homeNode = CreateBoard( boardRows, boardCols );

      PrintBoard( homeNode, boardRows, boardCols, gameWindow, termRows, termCols );

      mvwaddstr( gameWindow, 0, 0, "ORIGIN");
      refresh();
      

      while(1);

      break;

    case INVALID_STATE:
    case QUIT:

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
