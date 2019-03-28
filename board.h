/*
 *  CS201 - Portfolio Project
 *  Jonathan Blake - 11704031
 *  Terminal Connect Four
 *
 *  ======
 *
 *  BOARD.H
 *  contains game board graph and functions to create, edit, and remove graph
 *
 */


#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>


typedef enum { ONE, TWO, NONE, INVALID_PLAYER } PLAYER;

typedef struct boardnode {
  PLAYER * owner;
  struct boardnode * above;
  struct boardnode * below;
  struct boardnode * left;
  struct boardnode * right;
  struct boardnode * aboveLeft;
  struct boardnode * aboveRight;
  struct boardnode * belowLeft;
  struct boardnode * belowRight;
} BoardNode;


// Function prototypes

BoardNode * CreateBoard(int, int);
BoardNode * NewNode();

int DropToken(BoardNode *, int, PLAYER);
void PrintBoard(BoardNode *, int, int, WINDOW *, int, int, int);

int IsWinningMove(BoardNode *, int, PLAYER);

void FreeBoard(BoardNode *);


#endif  /*  BOARD_H  */
