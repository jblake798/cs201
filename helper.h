/*
 *  CS201 - Portfolio Project
 *  Jonathan Blake - 11704031
 *  Terminal Connect Four
 *
 *  ======
 *
 *  HELPER.H
 *  interface to helper functions for terminal management
 *
 */


#ifndef HELPER_H
#define HELPER_H


// Macros (Reasons for QUIT)

#define USER_QUIT    1


// Function prototypes

void Error_Quit(char * msg);
void Quit(int reason);
void UpdateTermSize(int * rows, int * cols);
void handler(int signum);
void CleanUp(void);


#endif  /*  HELPER_H  */
