//.c file which contains function definitions
#include "ChessUtilities.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>
// Using gotoxy(x,y) which sets the cursor on entered x and y coordinates
#define gotoxy(x,y) printf ( "\033[%d;%dH", (y), (x) )

// This function takes a string and a number and prints that string that number of times
void printStr ( char c[], int times )
{
    for ( int i=0 ; i<times ; i++ )
        printf ( "%s", c ) ;
}

// Overloading earlier function for different use cases
void printStr2 ( char c1[], int times1, char c2[], int times2 )
{
    for ( int i=0 ; i<times1 ; i++ )
        printf ( "%s", c1 ) ;
    for ( int i=0 ; i<times2 ; i++ )
        printf ( "%s", c2 ) ;
}

void printStr3 ( char c1[], int t1, char c2[], int t2, char c3[], int t3  )
{
    for ( int i=0 ; i<t1 ; i++ )
        printf ( "%s", c1 ) ;

    for ( int i=0 ; i<t2 ; i++ )
        printf ( "%s", c2 ) ;

    for ( int i=0 ; i<t3 ; i++ )
        printf ( "%s", c3 ) ;
}

// Instructions for proceeding with the game
void instructions ()
{
    printStr2 ( "\n", 1, "\t", 9 ) ;
    printf ( "INTRODUCTION\n" ) ;
    printStr2 ( "\t", 9, "-", 12 ) ;
    printf ( "\n\tAlt Chess is a  2-player game which is an alternate take on the game of chess. " ) ;
    printf ( "It consists of a 4X4 chessboard with 2 pawns, 1 knight and 1 king with \n\tthe priority of pawn being the " ) ;
    printf ( "\tlowest and king being the highest.\n" ) ;
    printf ( "\tThe objective is to kill as many opponent pieces as we can and get points according to the priority of the killed piece.\n" ) ;
    printStr2 ( "\n", 1, "\t", 9 ) ;
    printf ( "INSTRUCTIONS\n" ) ;
    printStr2 ( "\t", 9, "-", 12 ) ;
    printf ( "\n" ) ;
    printf ( "\t\t\t1. White always moves first, and players get alternate turns. Only one piece can be moved at a time.\n" ) ;
    printf ( "\t\t\t2. Pieces cannot move through other pieces (though the knight can jump over other pieces), " ) ;
    printf ( "and can never move onto a square with one of \n\t\t\t   their own pieces.\n" ) ;
    printf ( "\t\t\t3. The pawn is promoted to the rook of the same color once it reaches the opposite side of the board.\n" ) ;
    printf ( "\t\t\t4. Pawns can move only in the forward direction. They can only move to diagonal blocks towards the forward direction for " ) ;
    printf ( "taking opponents.\n" ) ;
    printf ( "\t\t\t5. Knight can move in a shape like L. Two spaces in forward, backward or side-to-side followed by a right or left turn.\n" ) ;
    printf ( "\t\t\t6. The rook moves in a continuous line forward, backward or side-to-side.\n" ) ;
    printf ( "\t\t\t7. King can move in all directions, one square at a time.\n" ) ;
    printf ( "\t\t\t8. For your convenience, we have also displayed the possible moves you can make.\n" ) ;
    printf ( "\t\t\t9. Killing the king does not mean the end of the game. The game ends only when the pieces of the same color are left on " ) ;
    printf ( "the board.\n" ) ;
    printf ( "\t\t\t10. If no piece is killed after 12 moves by any player and only one piece for each player is left, the condition of " ) ;
    printf ( "stalemate is achieved,\n\t\t\t    i.e the player with more points will be declared the winner.\n" ) ;
    printf ( "\t\t\t11. The priority of pieces is as followed:\n" ) ;
    printf ( "\t\t\t\t~King: Strongest, Points on killing King = 5\n" ) ;
    printf ( "\t\t\t\t~Rook: Intermediate, Points on killing Rook = 4\n" ) ;
    printf ( "\t\t\t\t~Knight: Intermediate, Points on killing Knight = 3\n" ) ;
    printf ( "\t\t\t\t~Pawn: Weakest, Points on killing Pawn = 1\n" ) ;

    printStr2 ( "\n", 3, "\t", 8 ) ;
    printf ( "*****Press any key to continue.***** \n" ) ;

    getch () ;
    system( "cls" ) ;
}

// Function to enter the names of both players
void display_name ( char p1[], char p2[] )
{
    printStr2 ( "\n", 6, "\t", 9 ) ;
    printf ( "Enter name of Player 1: " ) ;
    gets(p1) ;
    printStr2 ( "\n", 6, "\t", 9 ) ;
    printf ( "Enter name of Player 2: " ) ;
    gets(p2) ;
}

// Records points of both teams
void scorecounter ( int newrpos, int newcpos,square board[4][4], int *BPoints, int *WPoints)
{
    if ( board[newrpos][newcpos].content ) //If something needs to be killed
    {
        if( board[newrpos][newcpos].content->color == 1 )
            *BPoints += board[newrpos][newcpos].content->priority ;
        else if( board[newrpos][newcpos].content->color == 0 )
            *WPoints += board[newrpos][newcpos].content->priority ;
    }
}

// Displays points of both teams
void scoreboard ( int *BPoints, int *WPoints, char p1[], char p2[] )
{
    printStr2 ( "\n", 2, "\t", 9 ) ;
    printf ( "The current scores are:\n" ) ;
    printStr ( "\t", 12 ) ;
    printf ( "*%s (W) = %d\n", p1, *WPoints ) ;
    printStr ( "\t", 12 ) ;
    printf ( "*%s (B) = %d\n", p2, *BPoints ) ;
}

// Stores all possible moves for current piece to check if move entered by the user is a valid move
void pushInMoveStack ( moveStack *stack, int row, int column )
{
    move *toadd = (move*)malloc(sizeof(move)) ;
    toadd->c = column ;
    toadd->r = row ;
    toadd->next=NULL ;

    if ( !((*stack).top) )
        (*stack).top = toadd ;
    else
    {
        toadd->next = (*stack).top ;
        (*stack).top=toadd ;
    }
}

// New Record of all the remaining pieces for a player
void pushInLeftStack ( leftStack *stack, char toAdd[] )
{
    piecesLeft *toadd = (piecesLeft*)malloc(sizeof(piecesLeft)) ;
    strcpy ( toadd->pieceLeft, toAdd ) ;
    toadd->next=NULL ;

    if ( !((*stack).top) )
        (*stack).top = toadd ;
    else
    {
        toadd->next = (*stack).top ;
        (*stack).top=toadd ;
    }
}

// Function to empty a stack of moves
void emptyMoveStack ( moveStack *stack )
{
    (*stack).top=NULL ;
}


int remainingCtr ( leftStack *stack )
{
    piecesLeft *ctr = (*stack).top ;
    int count=0 ;

    while (ctr)
    {
        count++ ;
        ctr=ctr->next ;
    }

    return count ;
}

// Deleting killed pieces from left stack
void delFromLeftStack ( leftStack *stack, char toDel[] )
{
    piecesLeft *finder = (*stack).top ;

    if ( strcmp(finder->pieceLeft, toDel)==0 )
    {
        piecesLeft *todel = finder ;
        (*stack).top = ((*stack).top)->next ; ;
        free(todel) ;
    }
    else
    {
        while ( finder->next )
        {
            if ( strcmp(finder->next->pieceLeft, toDel)==0 )
            {
                piecesLeft *todel = finder->next ;
                finder->next=finder->next->next ;
                free(todel) ;
            }
            if ( finder->next )
                finder=finder->next ;
        }
    }
}

// Function to check if a piece is still alive
int pieceExists ( leftStack *stack, char toFind[] )
{
    piecesLeft *finder = (*stack).top ;

    while ( finder )
    {
        if ( strcmp(finder->pieceLeft, toFind)==0 )
            return 1 ;
        finder=finder->next ;
    }
    return 0 ;
    // Function returns 1 if piece is alive, 0 if it is not
}

// Function to change a value in a stack
void updateLeftStack ( leftStack *stack, char before[], char after[] )
{
    piecesLeft *finder = (*stack).top ;

    while ( finder )
    {
        if ( strcmp(finder->pieceLeft, before)==0 )
        {
            strcpy ( finder->pieceLeft, after ) ;
            break ;
        }
        finder = finder->next ;
    }
}

// Function to check if an entered move is valid
int checkIfValidMove ( moveStack *stack, int row, int col )
{
    move *finder=stack->top ;

    while ( finder )
    {
        if ( finder->c==col && finder->r==row )
            return 1 ;
        finder=finder->next ;
    }
    return 0 ;
    // Function returns 1 if there is a valid move, 0 if it is not
}

// Creating a new board and assigning starting positions to the pieces
void createboard ( leftStack *black, leftStack *white,square board[4][4],char empty[10],piece pieces[8])
{
    for ( int i=0 ; i<4 ; i++ )
    {
        for ( int j=0 ; j<4 ; j++ )
        {
            board[i][j].content=NULL ;
            strcpy ( board[i][j].item, empty ) ;
        }
    }
    for ( int i=0 ; i<4 ; i++ )
    {
        board[3][i].content=&(pieces[i]) ;
        strcpy ( board[3][i].item, pieces[i].name ) ;
        board[0][i].content=&(pieces[i+4]) ;
        strcpy ( board[0][i].item, pieces[i+4].name ) ;
    }
    for ( int i=0 ; i<8 ; i++ )
    {
        if ( i<4 )
            // Adding white pieces to white stack
            pushInLeftStack ( white, pieces[i].name ) ;
        else
            // Adding black pieces to black stack
            pushInLeftStack ( black, pieces[i].name ) ;
    }
}

// Displays current state of the board for a game
void displayboard (square board[4][4])
{
    printStr3 ( "\n", 2, "\t", 5, " ", 10 ) ;
    printStr3 ( "  0", 1, "\t", 3, "    1", 1 ) ;
    printStr3 ( "\t", 3, "    2", 1, "\t", 3 ) ;
    printf ( "    3" ) ;
    printStr3 ( "\t", 3, " ", 1, "\n", 1 ) ;
    printStr3 ( "\n", 1, "\t", 5, "-", 97 ) ;
    printf ( "\n" ) ;
    for ( int i=0 ; i<4 ; i++ )
    {
        printf ( "\t\t\t\t%d\t", i ) ;
        printf ( "|" ) ;
        for ( int j=0 ; j<4 ; j++ )
            printf ( "\t%s\t|", board[i][j].item ) ;
        printStr3 ( "\n", 1, "\t", 5, "-", 97 ) ;
        printf ( "\n" ) ;
    }
}

// Displays available pieces for Player 1 (White)
void displayPiecesP1 (piece pieces[8])
{
    for ( int i=1 ; i<=4 ; i++ )
    {
        if ( pieces[i-1].rowpos != -1 )
        {
            printStr ( "\t", 9 ) ;
            printf ( "%d. %s\n", i, pieces[i-1].name ) ;
        }
    }
    printf ( "\n" ) ;
}

// Displays available pieces for Player 2 (Black)
void displayPiecesP2 (piece pieces[8])
{
    for ( int i=5 ; i<=8 ; i++ )
    {
        if ( pieces[i-1].rowpos != -1 )
        {
            printStr ( "\t", 9 ) ;
            printf ( "%d. %s\n", i-4, pieces[i-1].name ) ;
        }
    }
    printf ( "\n" ) ;
}

// Function to check if a piece is at one of the edges of the board
int checkedgepiece ( int a )
{
    if ( a==0 )
        return 1 ;
    else if ( a==3 )
        return 2 ;
    else
        return 0 ;
    // Function returns 1 for left edge, 2 for right edge, 0 if not an edge piece
}

// Function to check if an enemy is present at an entered position
int findenemy ( int r, int c, int piececolor, square board[][4] )
{
    if ( board[r][c].content && board[r][c].content->color != piececolor )
        return 1 ;
    else
        return 0 ;
    // Function returns 1 if enemy is present on valid move, 0 if no enemy is present on valid move
}

// Message to enter full screen mode is displayed
void fullScreenMessage ()
{
    printStr2 ( "\n", 5, "\t", 9 ) ;
    printf ( "Please enter full screen\n" ) ;
    printStr3 ( "\t", 9, "~", 24, "\n", 1 ) ;
    printStr2 ( "\n", 10, "\t", 9 ) ;
    printf ( "Press any key to continue" ) ;
    getch () ;
    system ( "cls" ) ;
}

// Shows valid moves for all pieces
int showvalidmovesforuser ( moveStack *stack, piece a, int val, square board[4][4], char empty[] )
{
    int flag=0 ;

    // Code for white pawn movement
    if ( strcmp ( a.name, "Pawn 1 (W)" )==0 || strcmp ( a.name, "Pawn 2 (W)" )==0 )
    {
        // Forward movement
        if ( a.rowpos-1 >= 0 && strcmp ( board[a.rowpos-1][a.colpos].item, empty ) == 0 )
        {
            if ( val==1 )
            {
                printStr ( "\t", 11 ) ;
                printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos ) ;
            }
            pushInMoveStack( stack, a.rowpos-1, a.colpos ) ;
            flag=1 ;
        }

        // For pawn present at left edge (view in direction of forward movement)
        if ( checkedgepiece ( a.colpos ) == 1 )
        {
            // For attacking enemy using diagonal movement
            if ( a.colpos+1 >= 0 && a.rowpos-1 >= 0 && findenemy(a.rowpos-1, a.colpos+1, a.color, board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos+1 ) ;
                flag=1 ;
            }
        }

        // For pawn present at right edge (view in direction of forward movement)
        else if ( checkedgepiece ( a.colpos ) == 2 )
        {
            // For attacking enemy using diagonal movement
            if ( a.colpos-1 >= 0 && a.rowpos-1 >= 0 && findenemy(a.rowpos-1, a.colpos-1, a.color,board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos-1 ) ;
                flag=1 ;
            }
        }

        // For pawn present anywhere except the edges
        else
        {
            // For attacking enemy using diagonal movement
            if ( a.colpos+1 >= 0 && a.rowpos-1 >= 0 && findenemy(a.rowpos-1, a.colpos+1, a.color,board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos+1 ) ;
                flag=1 ;
            }

            // For attacking enemy using diagonal movement
            if ( a.colpos-1 >= 0 && a.rowpos-1 >= 0 && findenemy(a.rowpos-1, a.colpos-1, a.color,board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos-1 ) ;
                flag=1 ;
            }
        }
    }

    // Code for black pawn movement
    else if ( strcmp ( a.name, "Pawn 1 (B)" )==0 || strcmp ( a.name, "Pawn 2 (B)" )==0 ) // Code for pawn movement
    {
        // Forward Movement
        if ( a.rowpos+1 <= 3 && strcmp ( board[a.rowpos+1][a.colpos].item, empty ) == 0 )
        {
            if ( val==1 )
            {
                printStr ( "\t", 11 ) ;
                printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos ) ;
            }
            pushInMoveStack( stack, a.rowpos+1, a.colpos ) ;
            flag=1 ;
        }

        // For pawn present at left edge (view in direction of forward movement)
        if ( checkedgepiece ( a.colpos ) == 2 )
        {
            // For attacking enemy using diagonal movement
            if ( a.colpos-1 <= 3 && a.rowpos+1 <= 3 && findenemy(a.rowpos+1, a.colpos-1, a.color,board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos-1 ) ;
                flag=1 ;
            }
        }

        // For pawn present at right edge (view in direction of forward movement)
        else if ( checkedgepiece ( a.colpos ) == 1 )
        {
            // For attacking enemy using diagonal movement
            if ( a.colpos+1 <= 3 && a.rowpos+1 <= 3 && findenemy(a.rowpos+1, a.colpos+1, a.color,board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos+1 ) ;
                flag=1 ;
            }
        }

        // For pawn present anywhere except the edges
        else
        {
            // For attacking enemy using diagonal movement
            if ( a.colpos+1 >= 0 && a.rowpos+1 <= 3 && findenemy(a.rowpos+1, a.colpos+1, a.color,board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos+1 ) ;
                flag=1 ;
            }

            // For attacking enemy using diagonal movement

            if ( a.colpos-1 >= 0 && a.rowpos+1 >= 3 && findenemy(a.rowpos+1, a.colpos-1, a.color,board)==1 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos-1 ) ;
                flag=1 ;
            }
        }
    }

    // Code for King movement
    else if ( strcmp ( a.name, "King (W)" )==0 || strcmp ( a.name, "King (B)" )==0 ) // Code for King movement
    {
        //Forward movement for white king, backward for black king
        if ( (a.rowpos-1 >= 0 && a.rowpos-1<4  && strcmp ( board[a.rowpos-1][a.colpos].item, empty ) == 0)
             || (a.rowpos-1 >= 0&& a.rowpos-1<4 && findenemy(a.rowpos-1, a.colpos, a.color,board)==1) )
        {
            if ( val==1 )
            {
                printStr ( "\t", 11 ) ;
                printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos ) ;
            }
            pushInMoveStack( stack, a.rowpos-1, a.colpos ) ;
            flag=1 ;
        }

        // If white king is present at left edge, black king is at right edge
        if ( checkedgepiece ( a.colpos ) == 1 )
        {
            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos+1 >= 0 && a.rowpos-1 >= 0 && a.colpos+1 < 4 && a.rowpos-1<4 && findenemy(a.rowpos-1, a.colpos+1, a.color,board)==1)
               || (a.colpos+1 >= 0 && a.rowpos-1 >= 0 && a.colpos+1 < 4 && a.rowpos-1<4 && strcmp ( board[a.rowpos-1][a.colpos+1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos+1 ) ;
                flag=1 ;
            }
        }

        // If white king is present at right edge, black king is at left side
        else if ( checkedgepiece ( a.colpos ) == 2 )
        {
            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos-1 >= 0 && a.rowpos-1 >= 0 &&  a.colpos-1 < 4 && a.rowpos-1 < 4 && findenemy(a.rowpos-1, a.colpos-1, a.color,board)==1)
            || (a.colpos-1 >= 0 && a.rowpos-1 >= 0 &&  a.colpos-1 < 4 && a.rowpos-1 < 4 && strcmp ( board[a.rowpos-1][a.colpos-1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos-1 ) ;
                flag=1 ;
            }
        }

        // If the king ( white or black ) is present anywhere except the edges
        else
        {
            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos+1 >= 0 && a.rowpos-1 >= 0 && a.colpos+1 < 4 && a.rowpos-1 <4 && findenemy(a.rowpos-1, a.colpos+1, a.color,board)==1)
            || (a.colpos+1 >= 0 && a.colpos+1 < 4 && a.rowpos-1 <4 && a.rowpos-1 >= 0 && strcmp ( board[a.rowpos-1][a.colpos+1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos+1 ) ;
                flag=1 ;
            }

            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos-1 >= 0 && a.rowpos-1 >= 0 && a.colpos-1 < 4 && a.rowpos-1 <4 && findenemy(a.rowpos-1, a.colpos-1, a.color,board)==1)
            || (a.colpos-1 >= 0 && a.rowpos-1 >= 0 && a.colpos-1 < 4 && a.rowpos-1 <4  && strcmp ( board[a.rowpos-1][a.colpos-1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos-1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos-1, a.colpos-1 ) ;
                flag=1 ;
            }
        }

        // Right move for white king, left move for black king
        if ( (a.colpos+1 >= 0 && a.colpos+1<4 && strcmp ( board[a.rowpos][a.colpos+1].item, empty ) == 0)
            || (a.colpos+1 >= 0 && a.colpos+1<4 && findenemy(a.rowpos, a.colpos+1, a.color,board)==1) )
        {
            if ( val==1 )
            {
                printStr ( "\t", 11 ) ;
                printf ( "Position R:%d and C:%d \n", a.rowpos, a.colpos+1 ) ;
            }
            pushInMoveStack( stack, a.rowpos, a.colpos+1 ) ;
            flag=1 ;
        }

        // Left move for white king, right move for black king
        if ( (a.colpos-1 >= 0 && a.colpos-1<4 && strcmp ( board[a.rowpos][a.colpos-1].item, empty ) == 0)
            || (a.colpos-1 >= 0&& a.colpos-1<4 && findenemy(a.rowpos, a.colpos-1, a.color,board)==1) )
        {
            if ( val==1 )
            {
                printStr ( "\t", 11 ) ;
                printf ( "Position R:%d and C:%d \n", a.rowpos, a.colpos-1 ) ;
            }
            pushInMoveStack( stack, a.rowpos, a.colpos-1 ) ;
            flag=1 ;
        }

        // Backward movement for white king, forward for white king
        if ( (a.rowpos+1 >= 0 && a.rowpos+1<4 && strcmp ( board[a.rowpos+1][a.colpos].item, empty ) == 0)
            ||  (a.rowpos+1 >= 0 && a.rowpos+1<4 && findenemy(a.rowpos+1, a.colpos,a.color,board)==1) )
        {
            if ( val==1 )
            {
                printStr ( "\t", 11 ) ;
                printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos ) ;
            }
            pushInMoveStack( stack, a.rowpos+1, a.colpos ) ;
            flag=1 ;
        }

        // If white king is present at left edge, black king at right edge
        if ( checkedgepiece ( a.colpos ) == 1 )
        {
            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos+1 >= 0 && a.rowpos+1 >= 0 &&  a.colpos+1 <4 && a.rowpos+1<4 && findenemy(a.rowpos+1, a.colpos+1, a.color,board)==1)
               || (a.colpos+1 >= 0 && a.rowpos+1 >= 0 &&  a.colpos+1 <4 && a.rowpos+1<4  && strcmp ( board[a.rowpos+1][a.colpos+1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos+1 ) ;
                flag=1 ;
            }
        }

        // If white king is present at right edge, black king at left edge
        else if ( checkedgepiece ( a.colpos ) == 2 )
        {
            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos-1 >= 0 && a.colpos-1<4 && a.rowpos+1 >= 0 && a.rowpos+1<4 && findenemy(a.rowpos+1, a.colpos-1, a.color,board)==1)
                || (a.colpos-1 >= 0 && a.colpos-1<4  && a.rowpos+1 >= 0 && a.rowpos+1<4 && strcmp ( board[a.rowpos+1][a.colpos-1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos-1 ) ;
                flag=1 ;
            }
        }

        // If king ( white or black ) is present anywhere except the edges
        else
        {
            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos+1 >= 0 && a.colpos+1<4 && a.rowpos+1 >= 0&&  a.rowpos+1<4 && findenemy(a.rowpos+1, a.colpos+1, a.color,board)==1)
               || (a.colpos+1 >= 0 && a.colpos+1<4 && a.rowpos+1 >= 0 &&  a.rowpos+1<4 && strcmp ( board[a.rowpos+1][a.colpos+1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos+1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos+1 ) ;
                flag=1 ;
            }

            // Shows valid move one square ahead in the directions if either enemy is present or block is empty
            if ( (a.colpos-1 >= 0 && a.rowpos+1 >= 0 && a.colpos-1 < 4 && a.rowpos+1 < 4 && findenemy(a.rowpos+1, a.colpos-1, a.color,board)==1)
            || (a.colpos-1 >= 0 && a.rowpos+1 >= 0 && a.colpos-1 < 4 && a.rowpos+1 < 4  && strcmp ( board[a.rowpos+1][a.colpos-1].item, empty )==0) )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos+1, a.colpos-1 ) ;
                }
                pushInMoveStack( stack, a.rowpos+1, a.colpos-1 ) ;
                flag=1 ;
            }
        }
    }

    // Code for knight movement
    else if ( strcmp ( a.name, "Knight (W)" )==0 || strcmp ( a.name, "Knight (B)" )==0 )
    {
        for ( int i=1 ; i<3 ; i++ )
        {
            for(int j=1;j<3;j++)
            {
                if ( i!= j )
                {
                    if ( (a.rowpos+i <=3 && a.colpos-j>=0 ) && ( strcmp( board[a.rowpos+i][a.colpos-j].item, empty ) == 0
                        || findenemy ( a.rowpos+i, a.colpos-j, a.color, board ) == 1 ) )
                    {
                    if ( val==1 )
                    {
                        printStr ( "\t", 11 ) ;
                        printf ( "Position R:%d and C:%d \n", a.rowpos+i, a.colpos-j ) ;
                    }
                    pushInMoveStack( stack, a.rowpos+i, a.colpos-j ) ;
                    flag=1 ;
                    }
                    if ( (a.rowpos+i <=3 && a.colpos+j<=3 ) && ( strcmp( board[a.rowpos+i][a.colpos+j].item, empty ) == 0
                        || findenemy ( a.rowpos+i, a.colpos+j, a.color, board ) == 1 ) )
                    {
                        if ( val==1 )
                        {
                            printStr ( "\t", 11 ) ;
                            printf ( "Position R:%d and C:%d \n", a.rowpos+i, a.colpos+j ) ;
                        }
                        pushInMoveStack( stack, a.rowpos+i, a.colpos+j ) ;
                        flag=1 ;
                    }
                    if ( ( a.rowpos-i>=0 && a.colpos-j>=0 ) && (strcmp( board[a.rowpos-i][a.colpos-j].item, empty ) == 0
                        || findenemy(a.rowpos-i, a.colpos-j, a.color,board)==1 ) )
                    {
                        if ( val==1 )
                        {
                            printStr ( "\t", 11 ) ;
                            printf ( "Position R:%d and C:%d \n", a.rowpos-i, a.colpos-j ) ;
                        }
                        pushInMoveStack( stack, a.rowpos-i, a.colpos-j ) ;
                        flag=1 ;
                    }
                    if ( (a.rowpos-i >=0 && a.colpos+j<=3 ) && ( strcmp( board[a.rowpos-i][a.colpos+j].item, empty ) == 0
                        || findenemy(a.rowpos-i, a.colpos+j, a.color, board)==1 ) )
                    {
                        if ( val==1 )
                        {
                            printStr ( "\t", 11 ) ;
                            printf ( "Position R:%d and C:%d \n", a.rowpos-i, a.colpos+j ) ;
                        }
                        pushInMoveStack( stack, a.rowpos-i, a.colpos+j ) ;
                        flag=1 ;
                    }
                }
            }
        }
    }

    // This code applies to pawns promoted to rook
    else if ( strcmp ( a.name, "Rook 1 (W)")==0 || strcmp ( a.name, "Rook 2 (W)")==0 || strcmp ( a.name, "Rook 1 (B)")==0
            || strcmp ( a.name, "Rook 2 (B)")==0 )
    {
        for ( int i=a.colpos-1 ; i>=0 ; i-- )
            if (findenemy ( a.rowpos, i, a.color , board)==1 || strcmp ( board[a.rowpos][i].item, empty ) == 0 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos, i ) ;
                }
                pushInMoveStack( stack, a.rowpos, i ) ;
                flag=1 ;
                if ( findenemy ( a.rowpos, i, a.color,board)==1 )
                    break;
            }

        for ( int i=a.colpos+1 ; i<=3 ; i++ )
            if (findenemy ( a.rowpos, i, a.color,board)==1 || strcmp ( board[a.rowpos][i].item, empty ) == 0 )
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", a.rowpos, i ) ;
                }
                pushInMoveStack( stack, a.rowpos, i ) ;
                flag=1 ;
                if ( findenemy ( a.rowpos, i, a.color, board)==1 )
                    break;
            }

        for ( int j=a.rowpos-1 ; j>=0 ; j-- )
            if ( findenemy ( j, a.colpos, a.color,board)==1 || strcmp ( board[j][a.colpos].item, empty ) == 0)
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", j, a.colpos ) ;
                }
                pushInMoveStack( stack, j, a.colpos ) ;
                flag=1 ;
                if ( findenemy ( j, a.colpos, a.color,board)==1 )
                    break;
            }

        for ( int j=a.rowpos+1 ; j<=3 ; j++ )
            if ( findenemy ( j, a.colpos, a.color, board)==1 || strcmp ( board[j][a.colpos].item, empty ) == 0)
            {
                if ( val==1 )
                {
                    printStr ( "\t", 11 ) ;
                    printf ( "Position R:%d and C:%d \n", j, a.colpos ) ;
                }
                pushInMoveStack( stack, j, a.colpos ) ;
                flag=1 ;
                if ( findenemy ( j, a.colpos, a.color,board)==1 )
                    break;
            }
    }
    return flag ;
}

// Function to move a piece to a given position
void movepiece ( int a, int newrpos, int newcpos, leftStack *stack, int *stalemateCtr, square board[4][4], piece pieces[8], char empty[] )
{
    (*stalemateCtr)++ ;

    // If something needs to be killed
    if ( board[newrpos][newcpos].content )
    {
        printStr3 ( "\n", 1, "\t", 9, "~", 2 ) ;
        printf ( "Killed %s!~~\n", board[newrpos][newcpos].content->name ) ;

        // Killed piece deleted from record of remaining pieces for the player
        delFromLeftStack( stack, board[newrpos][newcpos].item ) ;

        *stalemateCtr=0 ;
        board[newrpos][newcpos].content->colpos=-1 ;
        board[newrpos][newcpos].content->rowpos=-1 ;
    }

    strcpy ( board[pieces[a].rowpos][pieces[a].colpos].item, empty ) ;
    board[pieces[a].rowpos][pieces[a].colpos].content=NULL ;
    pieces[a].rowpos=newrpos ;
    pieces[a].colpos=newcpos ;
    strcpy ( board[pieces[a].rowpos][pieces[a].colpos].item, pieces[a].name ) ;
    board[newrpos][newcpos].content=&(pieces[a]) ;
}

// Pawn promotes to rook once it reaches opposite end of the board
void promote ( piece *a, leftStack *stack,piece pieces[8])
{
    if ( strcmp ( (*a).name, "Pawn 1 (W)" )==0 )
    {
        strcpy ( (*a).name, "Rook 1 (W)" ) ;
        updateLeftStack ( stack, "Pawn 1 (W)", "Rook 1 (W)" ) ;
        pieces[0].priority = 4 ;
    }
    else if ( strcmp ( (*a).name, "Pawn 2 (W)" )==0 )
    {
        strcpy ( (*a).name, "Rook 2 (W)" ) ;
        updateLeftStack ( stack, "Pawn 2 (W)", "Rook 2 (W)" ) ;
        pieces[3].priority = 4 ;
    }
    else if ( strcmp ( (*a).name, "Pawn 1 (B)" )==0 )
    {
        strcpy ( (*a).name, "Rook 1 (B)" ) ;
        updateLeftStack ( stack, "Pawn 1 (B)", "Rook 1 (B)" ) ;
        pieces[4].priority = 4 ;
    }
    else if ( strcmp ( (*a).name, "Pawn 2 (B)" )==0 )
    {
        strcpy ( (*a).name, "Rook 2 (B)" ) ;
        updateLeftStack ( stack, "Pawn 2 (B)", "Rook 2 (B)" ) ;
        pieces[7].priority = 4 ;
    }
}

void declareWinner ( leftStack *black, leftStack *white, int *WPoints, int *BPoints, char p1[], char p2[] )
{
    // If only white pieces are present on the board
    if ( remainingCtr(black)==0 )
    {
        printStr2 ( "\n", 9, "\t", 9 ) ;
        printf ( "%s wins! \n\n\n\n", p1 ) ;
        return ;
    }
    // If only black pieces are present on the board
    else if ( remainingCtr(white)==0 )
    {
        printStr2 ( "\n", 9, "\t", 9 ) ;
        printf ( "%s wins! \n\n\n\n", p2 ) ;
        return ;
    }
    else
    {
        if ( *WPoints > *BPoints )
        {
            printStr2 ( "\n", 9, "\t", 9 ) ;
            printf ( "%s wins! \n\n\n\n", p1 ) ;
            return ;
        }
        else if ( *BPoints > *WPoints )
        {
            printStr2 ( "\n", 9, "\t", 9 ) ;
            printf ( "%s wins! \n\n\n\n", p2 ) ;
            return ;
        }
        else
        {
            printStr2 ( "\n", 9, "\t", 9 ) ;
            printf ( "Game tied! \n\n\n\n" ) ;
            return ;
        }
    }
}

// Prints the box in which piece number is entered
void  display_textbox ()
{
    // To fix the box at the entered coordinates
    gotoxy ( 1, 28 ) ;
    printStr3 ( "\t", 9, "+", 1, "-", 17 ) ;
    printf ( "+\n" ) ;
    gotoxy ( 1, 29 ) ;
    printStr3 ( "\t", 9, "|", 1, " ", 17 ) ;
    printf ( "|\n" ) ;
    gotoxy ( 1, 30 ) ;
    printStr3 ( "\t", 9, "+", 1, "-", 17 ) ;
    printf ( "+\n" ) ;
}


//When no moves are available for the player then their turn is skipped
int skipTurn ( moveStack *stack1, leftStack *stack2, int piececolor, piece pieces[8], square board[4][4], char empty[] )
{
    int ctr=0 ;
    // If color of the piece is white
    if ( piececolor==1 )
        for ( int i=0 ; i<4 ; i++ )
            // If a piece is killed or the pieces on board have no moves then, turn will be skipped
            if ( pieceExists( stack2, pieces[i].name)==0 || showvalidmovesforuser ( stack1, pieces[i],0, board, empty )==0)
                ctr++ ;
    //If color of the piece is black
    if ( piececolor==0 )
        for ( int j=4 ; j<8 ; j++ )
            if ( pieceExists( stack2, pieces[j].name)==0 || showvalidmovesforuser ( stack1, pieces[j], 0, board, empty )==0)
                ctr++ ;
    if ( ctr==4 )
    {
        printStr2 ( "\n", 2, "\t", 9 ) ;
        printf ( "No more moves. Turn Skipped. \n" ) ;
        printStr2 ( "\n", 1, "\t", 9 ) ;
        printf ( "**Press any key to continue**.\n" ) ;
        getch () ;
    }
    return ctr ;
}

//Experience of all group members
void experience()
{
    printStr2 ( "\n", 1, "\t", 10 ) ;
    printf ( "OUR EXPERIENCE\n" ) ;
    printStr2 ( "\n", 1, "\t", 3 ) ;
    printf ( "SHRADDHA:  " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf("An alternative that makes us use our minds! I had always loved real-time strategy games but creating one");
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "was never on my list. This project gave me an excellent opportunity to not only apply my knowledge but also " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "learn a lot of new concepts as well as get comfortable with the ones I always thought were difficult. One very important thing " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "which I learned during this project is how debugging at each step is necessary because in a very long code it is not " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "possible to find errors in each line. Hence, we all learned the concept of breakpoints and how they worked. This project taught " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "us all a lot and we learned how much hard work, patience and team-work it takes to complete a group project. " ) ;
    printStr2 ( "\n",2 ,"\t",3);
    printf ( "HRIDAY: " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf("We thought of AltChess as a refreshing, fast-paced take on the traditional game of chess, a game we all know ");
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "and love. We all thought it would be so much fun if we could put our own spin on it. With the tools we had learned, we could! " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "To see our ideas and our effort come to life was incredible and we hope you enjoy it!" ) ;
    printStr2 ( "\n",2 ,"\t",3);
    printf ( "KHUSHI: " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf("Chess is a two-player strategy  board  game played by millions of people worldwide. The idea of Alt-Chess ");
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "was originated with a simple idea on how to keep people engaged amidst their busy lives and take them back to their " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "childhood to reinstall the feeling of playing board games. Keeping the rules contemporary we aim at increasing the player's " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "concentration skills, problem-solving ability,planning and foresight and presence of mind. I genuinely hope " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "this game is enjoyed as much as we had fun designing it. " ) ;
    printStr2 ( "\n",2 ,"\t",3);
    printf ( "KAVYA: " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf("While growing up and playing chess at competitions up to state level, this one particular game never failed ");
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "to entertain me. But I always played it the traditional way, on a board with actual pieces. Never really tried it on a " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "computer or mobile phone, even though it did fascinate me. Now, being a CS student, attempting and successfully creating this " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "simple smaller version of such an amazing game has enlightened me. Initially, I did think that it won't be possible " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "for my team to make something like this, but with a positive mindset and lots of trials and errors, here we are with our " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "own AltChess. " ) ;
    printStr2 ( "\n",2 ,"\t",3);
    printf ( "MEHAK:  " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf("I recognized my love for coding when I was in school but hadn't really had an opportunity, until now, to");
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "refine it. To be honest, before making this project, I was not familiar with the rules of chess,but when we decided to " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "make a project on it, I thought this might be a good learning experience for me and I might even learn how to play chess. " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "I learned a lot, not just about chess but also about teamwork and programming. I learned how to build the code " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "in debug mode and use the debugger by adding breakpoints, stepwise execution, watching variable values at run time, and hence, " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "finding the bugs which were very difficult to find otherwise. While making this project, we faced a lot of obstacles " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "like when we changed the code even slightly, there would be some unexpected errors but after plenty of trials and " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "debugging, we were finally able to create this game. It was interesting to understand how complex and lengthy codes are " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "organized into the different header(.h files) and source(.c files) code files. Overall, now we understand how " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "much effort it takes and how much patience is required to develop a simple-looking application or game. " ) ;
    printStr2 ( "\n",1 ,"\t",4);
    printf ( "This game has been made with continuous efforts from our entire team, so hope you enjoy it!") ;
    printStr2 ( "\n", 3, "\t", 9 ) ;
    printf ( "*****Press any key to continue.***** \n" ) ;
    getch () ;
    system ( "cls" ) ;
}









