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
  for ( int i = 0 ; i < column ; i++ ) {
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


/*  Check if token would win in given game board graph structure  */

int IsWinningMove(BoardNode * homeNode, int column, PLAYER player)
{
  // pointer for graph navigation
  BoardNode * currentNode = homeNode;

  // navigate to desired column
  for ( int i = 0 ; i < column ; i++ ) {
    currentNode = currentNode->right;

    // if column does not exist, report error
    if ( currentNode == NULL ) return -1;
  }

  // move up column until empty node found. search surroundings if found
  while ( currentNode != NULL ) {
    
    if ( *currentNode->owner == NONE ) {

      // save node to focus on
      BoardNode * focusNode = currentNode;

      // check vertical
      int i = 0;
      currentNode = currentNode->below;
      while ( ( currentNode != NULL ) && ( *currentNode->owner == player ) && ( i < 3 ) ) {
	currentNode = currentNode->below;
	++i;
      } if ( i == 3 ) return 1;

      
      // reset currentNode and coutners
      currentNode = focusNode;
      i = 0;
      int j = 0;

      // check horizontal
      // check left
      currentNode = currentNode->left;
      while ( ( currentNode != NULL ) && ( *currentNode->owner == player ) && ( i < 3 ) ) {
	currentNode = currentNode->left;
	++i;
      }
      // check right
      currentNode = focusNode->right;
      while ( ( currentNode != NULL ) && ( *currentNode->owner == player ) && ( j < 3 ) ) {
	currentNode = currentNode->right;
	++j;
      }
      // compute total in a row and return if 4 in a row
      if ( ( i + j + 1 ) > 3 ) return 1;

      
      // reset currentNode and coutners
      currentNode = focusNode;
      i = 0;
      j = 0;

      // check backslash diagonal
      // check above left
      currentNode = currentNode->aboveLeft;
      while ( ( currentNode != NULL ) && ( *currentNode->owner == player ) && ( i < 3 ) ) {
	currentNode = currentNode->aboveLeft;
	++i;
      }
      // check below right
      currentNode = focusNode->belowRight;
      while ( ( currentNode != NULL ) && ( *currentNode->owner == player ) && ( j < 3 ) ) {
	currentNode = currentNode->belowRight;
	++j;
      }
      // compute total in a row and return if 4 in a row
      if ( ( i + j + 1 ) > 3 ) return 1;

      
      // reset currentNode and coutners
      currentNode = focusNode;
      i = 0;
      j = 0;

      // check forwardslash diagonal
      // check above right
      currentNode = currentNode->aboveRight;
      while ( ( currentNode != NULL ) && ( *currentNode->owner == player ) && ( i < 3 ) ) {
	currentNode = currentNode->aboveRight;
	++i;
      }
      // check below left
      currentNode = focusNode->belowLeft;
      while ( ( currentNode != NULL ) && ( *currentNode->owner == player ) && ( j < 3 ) ) {
	currentNode = currentNode->belowLeft;
	++j;
      }
      // compute total in a row and return if 4 in a row
      if ( ( i + j + 1 ) > 3 ) return 1;


      // if reaches here, not a winning move
      return 0;

    }
    currentNode = currentNode->above;
  }

  // catch if pointer reached top of column without finding empty node
  if ( currentNode == NULL ) return -1;

  // code should never reach this point
  return -2;

}


/*  Explores board recursively, viewing future moves to deliver best play decision  */

int WeakNegamax(BoardNode * homeNode, int column)
{
  return 0;
}


/*  Initiates negamax function to deliver best play decision  */

int AIDecision(BoardNode * homeNode, int columns)
{
  int decision = 0;
  int score = 0;

  // iterate through columns
  for ( int i = 0 ; i < columns ; i++ ) {
    
    // calculate column score
    score = WeakNegamax( homeNode, i );
    
    // if score leads to win, return decision
    if ( result == 1 ) return i;

    // else, if it leads to a tie, decide to play there,
    // but iterate again to look for better option
    else if ( result == 0 ) decision = i;
  }

  // return the decision
  return decision;
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
  int homeNodeY = ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 );

  // bound to prevent ncurses print error
  if ( homeNodeX < 0 ) homeNodeX = 0;
  if ( homeNodeY > ( termRows - 1 ) ) homeNodeY = ( termRows - 1 );

  // set incrementers for print corrdinates
  int x = homeNodeX;
  int y = homeNodeY;

  // define color options if available
  if ( has_colors() == TRUE ) {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
  }

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
	
	if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	mvwaddch( window, y, x++, '1' );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	
      } else if ( *currentNode->owner == TWO ) {
	
       	if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
	mvwaddch( window, y, x++, '2' );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );
	
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
