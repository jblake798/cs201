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

  // Clean up ncurses gui

  CleanUp();
  
    
  // Output error message and exit

  perror(msg);
  
  exit(EXIT_FAILURE);
  
}


/*  Quit successfully  */

void Quit(int reason)
{

  // Clean up ncurses gui

  CleanUp();
  

  // Output farewell message

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


  // Get terminal size

  if ( ioctl(0, TIOCGWINSZ, &ws) < 0 ) {
    perror("couldn't get window size");
    exit(EXIT_FAILURE);
  }
    

  // Update globals

  *rows = ws.ws_row;
  *cols = ws.ws_col;
  
}


/*  Signal handler  */

void handler(int signum)
{

  // Switch on signal number

  switch ( signum ) {
      
  case SIGTERM:
  case SIGINT:
  default:

    // Clean up ncurses gui
    
    CleanUp();
    
    
    exit(EXIT_SUCCESS);

  }
  
}


/*  Clean up ncurses windows  */

void CleanUp ()
{
    
  extern WINDOW * gameWindow;
  extern int oldcur;
  extern BoardNode * homeNode;

  delwin(gameWindow);
  curs_set(oldcur);
  endwin();
  refresh();
  FreeBoard(homeNode);
  
}
