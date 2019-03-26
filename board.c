/*
 *  CS201 - Portfolio Project
 *  Jonathan Blake - 11704031
 *  Terminal Connect Four
 *
 *  ======
 *
 *  BOARD.C
 *  contains game board graph and functions to create, edit, and remove graph
 *
 */


#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <curses.h>

#include "helper.h"
#include "board.h"


/*  Create game board graph structure  */

BoardNode * CreateBoard(int rows, int columns)
{
  // create homeNode (what board will be referenced by)
  BoardNode * homeNode = NewNode();

  // pointers for graph navigation
  BoardNode * rootColNode = homeNode;
  BoardNode * currentNode = NULL;
  BoardNode * leftHelperNode = NULL;
  BoardNode * newNode = NULL;

  for ( int j = 0 ; j < columns ; j++ ) {

    // move pointer to root of new column
    currentNode = rootColNode;

    for ( int i = 0 ; i < ( rows - 1 ) ; i++ ) {

      // create new node
      newNode = NewNode();

      // link to current node below
      newNode->below = currentNode;
      currentNode->above = newNode;

      // link to nodes in left column if left column present
      if ( leftHelperNode != NULL ) {
	// move helper node up one
	leftHelperNode = leftHelperNode->above;

	// link new node to below left and left
	newNode->belowLeft = leftHelperNode->below;
	leftHelperNode->below->aboveRight = newNode;

	newNode->left = leftHelperNode;
	leftHelperNode->right = newNode;

	// if node exists, link new node to above left
	if ( leftHelperNode->above != NULL ) {
	  newNode->aboveLeft = leftHelperNode->above;
	  leftHelperNode->above->belowRight = newNode;
	}
      }

      // move pointer to new node
      currentNode = newNode;
    }

    // start new column to right if necessary
    if ( j != ( columns - 1 ) ) {
      
      // create root of next column
      newNode = NewNode();

      // link new node to left and above left
      newNode->left = rootColNode;
      rootColNode->right = newNode;

      newNode->aboveLeft = rootColNode->above;
      rootColNode->above->belowRight = newNode;
    
      // move root column pointer and helper over one
      leftHelperNode = rootColNode;
      rootColNode = newNode;
    }

  }

  // return homeNode of graph to user
  return homeNode;
  
}


/*  Create new node for game board graph structure  */

BoardNode * NewNode()
{
  // allocate memory for node
  BoardNode * newNode = (BoardNode *) malloc( sizeof(BoardNode) );

  // allocate memory for player variable
  newNode->owner = (PLAYER *) malloc( sizeof(PLAYER) );
  *newNode->owner = NONE;

  // initialize pointers to NULL
  newNode->above      = NULL;
  newNode->below      = NULL;
  newNode->left       = NULL;
  newNode->right      = NULL;
  newNode->aboveLeft  = NULL;
  newNode->aboveRight = NULL;
  newNode->belowLeft  = NULL;
  newNode->belowRight = NULL;

  return newNode;
}


/*  Drop token in game board graph structure  */

int DropToken(BoardNode * homeNode, int column, PLAYER player)
{
  // pointer for graph navigation
  BoardNode * currentNode = homeNode;

  // navigate to desired column
  for ( int i = 1 ; i < column ; i++ ) {
    currentNode = currentNode->right;

    // if column does not exist, report error
    if ( currentNode == NULL ) return -1;
  }

  // move up column until empty node found. change ownership when found
  while ( currentNode != NULL ) {
    if ( *currentNode->owner == NONE ) {
      *currentNode->owner = player;
      return 1;
    }
    currentNode = currentNode->above;
  }

  // catch if pointer reached top of column without finding empty node
  if ( currentNode == NULL ) return 0;

  // code should never reach this point
  return -2;

}


/*  Print game board graph structure  */

void PrintBoard(BoardNode * homeNode, int boardRows, int boardCols,
		WINDOW * window, int termRows, int termCols,
		int cursorCol)
{
  // pointers for graph navigation
  BoardNode * currentNode = homeNode;
  BoardNode * nextRowNode;

  // calculate location of homeNode (X, Y)
  int homeNodeX = ( ( termCols / 2 ) - ( boardCols ) );
  int homeNodeY = ( ( termRows / 2 ) + ( boardRows / 2 ) );

  // bound to prevent ncurses print error
  if ( homeNodeX < 0 ) homeNodeX = 0;
  if ( homeNodeY > ( termRows - 1 ) ) homeNodeY = ( termRows - 1 );

  // set incrementers for print corrdinates
  int y = homeNodeY;
  int x = homeNodeX;

  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  // print cursor to screen first
  // keep column within bounds
  if ( cursorCol < 0 ) cursorCol = 0;
  if ( cursorCol > ( boardCols - 1 ) ) cursorCol = ( boardCols - 1 );

  // print cursor row
  for ( int i = 0 ; i < boardCols ; i++ ) {
    if ( x != homeNodeX ) mvwaddch( window, y, x++, ' ' );
    if ( i == cursorCol ) mvwaddch( window, y, x++, '^' );
    else mvwaddch( window, y, x++, ' ' );
  }

  // if top of window has not been reached, increment y
  if ( y < termRows ) --y;

  // reset x to start of row
  x = homeNodeX;  

  // progress through graph, printing owners as you go
  while ( currentNode != NULL ) {
    
    // remember next column
    nextRowNode = currentNode->above;
        
    while ( currentNode != NULL ) {

      // print space if not the first character
      if ( x != homeNodeX ) mvwaddch( window, y, x++, ' ' );
      
      // print owner in next space
      if ( *currentNode->owner == ONE ) {
	
	attron( COLOR_PAIR(1) );
	mvwaddch( window, y, x++, '1' );
	attroff( COLOR_PAIR(1) );
	
      } else if ( *currentNode->owner == TWO ) {
	
       	attron( COLOR_PAIR(2) );
	mvwaddch( window, y, x++, '2' );
	attroff( COLOR_PAIR(2) );
	
      } else
	mvwaddch( window, y, x++, '0' );

      // move to next node
      currentNode = currentNode->right;
    }

    refresh();

    // if top of window has not been reached, increment y
    if ( y < termRows ) --y;

    // reset x to start of row
    x = homeNodeX;

    // move to next column
    currentNode = nextRowNode;
  }
  
  return;
}


/*  Check for winner in game board graph structure  */

PLAYER IsWinner(BoardNode * homeNode)
{
  PLAYER winner = NONE;
  return winner;
}


/*  Free memory used by game board graph structure  */

void FreeBoard(BoardNode * homeNode)
{
  // pointers for graph navigation
  BoardNode * currentNode = homeNode;
  BoardNode * nextNode;
  BoardNode * nextColNode;

  // progress through graph, delete every node passed
  while ( currentNode != NULL ) {
    
    // remember next column
    nextColNode = currentNode->right;
    
    while ( currentNode != NULL ) {
      // remember next node
      nextNode = currentNode->above;

      // delete current node
      free( currentNode->owner );
      free( currentNode );

      // move to next node
      currentNode = nextNode;
    }

    // move to next column
    currentNode = nextColNode;
  }

  return;
  
}
