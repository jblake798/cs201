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

  for ( int j = 0 ; j < ( columns - 1 ) ; j++ ) {

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
    if ( j != ( columns - 2 ) ) {
      
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
		WINDOW * window, int termRows, int termCols)
{
  // pointers for graph navigation
  BoardNode * currentNode = homeNode;
  BoardNode * nextRowNode;

  // string to hold row values
  char rowString[ boardCols * 2 ];

  // calculate location of homeNode (X, Y)
  int homeNodeX = ( ( termCols / 2 ) - ( boardCols / 2 ) );
  int homeNodeY = ( ( termRows / 2 ) - ( boardRows / 2 ) );

  // bound from negative to prevent ncurses print error
  if ( homeNodeX < 0 ) homeNodeX = 0;
  if ( homeNodeY < 0 ) homeNodeY = 0;

  // set incrementer for printing rows
  int y = homeNodeY;

  // progress through graph, delete every node passed
  while ( currentNode != NULL ) {
    
    // remember next column
    nextRowNode = currentNode->above;

    // reset rowString
    rowString[0] = '\0';
    
    while ( currentNode != NULL ) {
      
      // add node owner to string
      if ( *currentNode->owner == ONE ) strcat( rowString, "A " );
      if ( *currentNode->owner == TWO ) strcat( rowString, "B " );
      else strcat( rowString, "0 " );

      // move to next node
      currentNode = currentNode->above;
    }

    // print created row string to window
    mvwaddstr( window, y, homeNodeX, rowString );
    refresh();

    // if top of window has not been reached, increment y
    if ( y < termRows ) ++y;

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
