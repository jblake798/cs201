/*
 *  CS201 - Portfolio Project
 *  Jonathan Blake - 11704031
 *  Terminal Connect Four
 *
 *  ======
 *
 *  HELPER.C
 *  helper functions for terminal management
 *
 */


#include <stdlib.h>

#include <sys/ioctl.h>
#include <signal.h>

#include <curses.h>

#include "helper.h"
#include "board.h"


/*  Quit on error  */

void Error_Quit(char * msg)
{

  // clean up ncurses gui
  CleanUp();
    
  // output error message and exit
  perror(msg);
  
  exit(EXIT_FAILURE);
  
}


/*  Quit successfully  */

void Quit(int reason)
{

  // clean up ncurses gui
  CleanUp();

  // output farewell message
  switch ( reason ) {
  default:
    printf("\nThanks for playing! Goodbye!\n");
    break;
  }

  exit(EXIT_SUCCESS);
  
}


/*  Returns the x-y size of the terminal  */

void UpdateTermSize(int * rows, int * cols)
{

  struct winsize ws;

  // get terminal size
  if ( ioctl(0, TIOCGWINSZ, &ws) < 0 ) {
    perror("couldn't get window size");
    exit(EXIT_FAILURE);
  }
    
  // update globals
  *rows = ws.ws_row;
  *cols = ws.ws_col;
  
}


/*  Signal handler  */

void handler(int signum)
{

  // switch on signal number
  switch ( signum ) {
      
  case SIGTERM:
  case SIGINT:
  default:

    // clean up ncurses gui    
    CleanUp();
        
    exit(EXIT_SUCCESS);

  }
  
}


/*  Clean up ncurses windows  */

void CleanUp ()
{
  
  extern BoardNode * homeNode;
  extern WINDOW * gameWindow;
  extern int oldcur;

  if ( gameWindow != NULL ) {
    delwin(gameWindow);
    curs_set(oldcur);
    endwin();
    refresh();
  } gameWindow = NULL;

  if ( homeNode != NULL ) FreeBoard(homeNode);
  homeNode = NULL;
    
}
