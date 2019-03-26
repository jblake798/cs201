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
  BoardNode * newNode = (BoardNode *) malloc( sizeof(BoardNode) );
  newNode->owner = (PLAYER *) malloc( sizeof(PLAYER) );
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
