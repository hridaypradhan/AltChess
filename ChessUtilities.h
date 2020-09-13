//.h file for chess functions
#include "HelperFunctions.h"

//STRUCTURES
// Definition for each piece
typedef struct piece
{
    char name[25] ;
    int priority ;
    int rowpos ;
    int colpos ;
    int color ;
    // 1 stands for white, 0 for black
} piece ;

typedef struct square
{
    char item[15] ;
    piece *content ;
} square ;

typedef struct move
{
    int r, c ;
    // r stands for row, c for column
    struct move *next ;
} move ;

// stores all the possible moves for the current piece to check if the move entered by the user is a valid move
typedef struct moveStack
{
    move *top ;
} moveStack ;

typedef struct piecesLeft
{
    char pieceLeft[15] ;
    struct piecesLeft *next ;
} piecesLeft ;

// Record of all the remaining pieces for a player
typedef struct leftStack
{
    piecesLeft *top ;
} leftStack ;

//FUNCTIONS

void scorecounter ( int newrpos, int newcpos, square board[4][4], int *BPoints, int *WPoints ) ;
void pushInMoveStack ( moveStack *stack, int row, int column ) ;
void pushInLeftStack ( leftStack *stack, char toAdd[] ) ;
void emptyMoveStack ( moveStack *stack ) ;
int remainingCtr ( leftStack *stack ) ;
void delFromLeftStack ( leftStack *stack, char toDel[] ) ;
int pieceExists ( leftStack *stack, char toFind[] ) ;
void updateLeftStack ( leftStack *stack, char before[], char after[] );
int checkIfValidMove ( moveStack *stack, int row, int col ) ;
void createboard ( leftStack *black, leftStack *white, square board[4][4], char empty[10], piece pieces[8] ) ;
void displayboard ( square board[4][4] ) ;
void displayPiecesP1 ( piece pieces[8] ) ;
void displayPiecesP2 ( piece pieces[8] ) ;
int checkedgepiece ( int a ) ;
int findenemy ( int r, int c, int piececolor, square board[][4] ) ;
int showvalidmovesforuser ( moveStack *stack, piece a , int val, square board[4][4], char empty[10] ) ;
void movepiece ( int a, int newrpos, int newcpos, leftStack *stack, int *stalemateCtr, square board[4][4], piece pieces[8], char empty[] ) ;
void promote ( piece *a, leftStack *stack, piece pieces[8] ) ;
void declareWinner ( leftStack *black, leftStack *white, int *WPoints, int *BPoints, char[], char[] ) ;
int skipTurn ( moveStack *stack1, leftStack *stack2, int piececolor, piece pieces[8], square board[4][4], char empty[] ) ;


