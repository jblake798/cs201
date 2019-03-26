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

#include <curses.h>

#include "helper.h"
#include "board.h"


/*  Create game board graph structure  */

BoardNode * CreateBoard(int rows, int columns)
{

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

void PrintBoard(BoardNode * homeNode, int boardRows, int boardCols, WINDOW * window, int termRows, int termCols)
{

}


/*  Check for winner in game board graph structure  */

PLAYER IsWinner(BoardNode * homeNode)
{

}


/*  Free memory used by game board graph structure  */

void FreeBoard(BoardNode * homeNode)
{

}
