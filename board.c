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


/*  Check if column is playable in game board graph structure  */

int CanPlay(int * height, int boardRows, int column)
{

  if ( height[column] < boardRows ) return 1;
  else return 0;

}


/*  Check if token would win in given game board graph structure  */

int IsWinningMove(int ** board, int * height, int boardRows, int boardCols, int column, int player)
{
  
  // check for vertical alignments
  if( ( height[column] >= 3 )
      && ( board[column][height[column]-1] == player )
      && ( board[column][height[column]-2] == player )
      && ( board[column][height[column]-3] == player ) )
    return 1;
  
  // Iterate on horizontal (dy = 0) or two diagonal directions (dy = -1 or dy = 1).
  for ( int dy = -1 ; dy <= 1 ; dy++ ) {
    
    int inOrder = 0;
    
    // count continuous stones of current player on the left, then right of the played column.
    for ( int dx = -1 ; dx <= 1 ; dx += 2 ) {
      
      int x = column + dx;
      int y = height[column] + dx * dy;
      while ( ( x >= 0 )
	      && ( x < boardCols )
	      && ( y >= 0 )
	      && ( y < boardRows )
	      && ( board[x][y] == player ) ) {
	x += dx;
	y += dx * dy;
	inOrder++;
      }

      // there is an aligment if at least 3 other stones of the current user  
      if( inOrder >= 3 ) return 1;
      
    }
        
  }
  
  return 0;

}


/*  Explores board recursively, viewing future moves to deliver best play decision  */

int negamax(int ** board, int * height, int move, int boardRows, int boardCols, int player, int alpha, int beta)
{
  
  // if tie, return 0
  if ( move == ( boardRows * boardCols ) )
    return 0;
  
  // check if current player will win the next move
  for ( int i = 0 ; i < boardCols ; i++ )
    if ( ( CanPlay( height, boardRows, i) == 1 ) && ( IsWinningMove( board, height, boardRows, boardCols, i, player ) == 1 ) )
      return ( ( ( boardRows * boardCols ) + 1 - move ) / 2 );

  // prune branch of recursion graph if necessary
  int max = ( ( ( boardRows * boardCols ) - 1 - move ) / 2 );
  if ( beta > max ) {
    beta = max;
    if ( alpha >= beta )
      return beta;
  }

  // compute the score of all possible next moves and keep the best one
  for ( int i = 0 ; i < boardCols ; i++ )
    
    if ( CanPlay( height, boardRows, i ) == 1 ) {
      
      // create board with set dimensions
      int **newBoard = (int **) malloc( boardCols * sizeof(int*) );
      for ( int j = 0 ; j < boardCols ; j++ )
	newBoard[j] = (int *) malloc( boardRows * sizeof(int) );
      for ( int j = 0 ; j < boardCols ; j++ )
	for ( int k = 0 ; k < boardRows ; k++ )
	  newBoard[j][k] = board[j][k];

      int *newHeight = (int *) malloc( boardCols * sizeof(int) );
      for ( int j = 0 ; j < boardCols ; j++ )
	newHeight[j] = height[j];

      // place token in next spot, creating next node in exploration tree
      if ( player == 1 ) newBoard[i][newHeight[i]] = 1;
      else if ( player == 2 ) newBoard[i][newHeight[i]] = 2;
      newHeight[i]++;
      move++;

      // switch turn before passing into negamax recursive function
      if ( player == 2 ) player = 1;
      else if ( player == 1 ) player = 2;

      // find score for current board setup
      int score = -negamax( newBoard, newHeight, move, boardRows, boardCols, player, -beta, -alpha );
      
      // free boards
      for ( int j = 0 ; j < boardCols ; j++ )
	free( newBoard[j] );
      free( newBoard );

      free( newHeight );

      // return score if greater than beta 
      if ( score >= beta ) return score;

      // if not greater, set alpha to score
      if ( score > alpha ) alpha = score;
      
    }

  return alpha;
  
}


/*  Wrapper function for managing AI choice and negamax function  */

int AIDecision(int ** board, int * height, int boardRows, int boardCols, int move)
{
  int decision = 0;
  int score = 0;
  int oldScore = -boardRows*boardCols;

  // check if current player will win the next move
  for ( int i = 0 ; i < boardCols ; i++ )
    if ( ( CanPlay( height, boardRows, i) == 1 ) && ( IsWinningMove( board, height, boardRows, boardCols, i, 2 ) == 1 ) )
      return i;
      

  // iterate through columns looking for next best move
  for ( int i = 0 ; i < boardCols ; i++ ) {

    if ( CanPlay( height, boardRows, i ) == 1 ) {

      // create board with set dimensions
      int **newBoard = (int **) malloc( boardCols * sizeof(int*) );
      for ( int j = 0 ; j < boardCols ; j++ )
	newBoard[j] = (int *) malloc( boardRows * sizeof(int) );
      for ( int j = 0 ; j < boardCols ; j++ )
	for ( int k = 0 ; k < boardRows ; k++ )
	  newBoard[j][k] = board[j][k];

      int *newHeight = (int *) malloc( boardCols * sizeof(int) );
      for ( int j = 0 ; j < boardCols ; j++ )
	newHeight[j] = height[j];

      // place potential token decision
      newBoard[i][newHeight[i]] = 2;
      newHeight[i]++;

      // calculate score for column
      score = -negamax( newBoard, newHeight, move+1, boardRows, boardCols, 1, (boardCols*boardRows/2), (-boardCols*boardRows/2) );

      // free function
      for ( int j = 0 ; j < boardCols ; j++ )
	free( newBoard[j] );
      free( newBoard );

      free( newHeight );

    }
    
    // if score leads to win, return decision
    if ( score > oldScore )
      decision = i;
    oldScore = score;
    
  }

  return decision;

}


/*  Print game board graph structure  */

void PrintBoard(int ** board, int boardRows, int boardCols,
		     WINDOW * window, int termRows, int termCols,
		     int cursorCol)
{
  
  // calculate location of homeNode (X, Y)
  int homeX = ( ( termCols / 2 ) - ( boardCols ) );
  int homeY = ( ( termRows / 2 ) + ( boardRows / 2 ) + 1 );

  // bound to prevent ncurses print error
  if ( homeX < 0 ) homeX = 0;
  if ( homeY > ( termRows - 1 ) ) homeY = ( termRows - 1 );

  // set incrementers for print corrdinates
  int x = homeX;
  int y = homeY;

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
    if ( x != homeX ) mvwaddch( window, y, x++, ' ' );
    if ( i == cursorCol ) mvwaddch( window, y, x++, '^' );
    else mvwaddch( window, y, x++, ' ' );
  }

  // if top of window has not been reached, increment y
  if ( y < termRows ) --y;

  // reset x to start of row
  x = homeX;  

  // progress through graph, printing owners as you go
  for ( int i = 0 ; i < boardRows ; i++ ) {
            
    for ( int j = 0 ; j < boardCols ; j++ ) {

      // print space if not the first character
      if ( x != homeX ) mvwaddch( window, y, x++, ' ' );
      
      // print owner in next space
      if ( board[j][i] == 1 ) {
	
	if ( has_colors() == TRUE ) attron( COLOR_PAIR(1) );
	mvwaddch( window, y, x++, '1' );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(1) );
	
      } else if ( board[j][i] == 2 ) {
	
       	if ( has_colors() == TRUE ) attron( COLOR_PAIR(2) );
	mvwaddch( window, y, x++, '2' );
	if ( has_colors() == TRUE ) attroff( COLOR_PAIR(2) );
	
      } else
	mvwaddch( window, y, x++, '0' );
    }

    refresh();

    // if top of window has not been reached, increment y
    if ( y < termRows ) --y;

    // reset x to start of row
    x = homeX;

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
