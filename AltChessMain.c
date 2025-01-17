//.c file which contains main()
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <malloc.h>
#include "ChessUtilities.h"

// Using gotoxy(x,y) which sets the cursor on entered x and y coordinates
#define gotoxy(x,y) printf ( "\033[%d;%dH", (y), (x) )

int main ()
{
    piece pieces[8] = {
                    // { name, priority, row position, column position, color }
                    { "Pawn 1 (W)", 1, 3, 0, 1 },
                    { "Knight (W)", 3, 3, 1, 1 },
                    { "King (W)", 5, 3, 2, 1 },
                    { "Pawn 2 (W)", 1, 3, 3, 1 },
                    { "Pawn 1 (B)", 1, 0, 0, 0 },
                    { "Knight (B)", 3, 0, 1, 0 },
                    { "King (B)", 5, 0, 2, 0 },
                    { "Pawn 2 (B)", 1, 0, 3, 0 },
                  } ;
    // 0=WPawn1, 1=WKnight, 2=WKing, 3=WPawn2, 4=BPawn1, 5=BKnight, 6=BKing, 7=BPawn2

    char empty[10] = "         ", p1Name[20], p2Name[20] ;

    square board[4][4] ;

    int stalemateCtr=0, WPoints = 0, BPoints = 0 ;

    // Message to enter full screen mode is displayed
    fullScreenMessage () ;

    // Instructions required to play the game are displayed
    instructions () ;
    experience () ;

    int ans=0, r, c, movesExist, forfeit=0, promotedOrNot[4] = { 0 } ;  // 0=WP1, 1=WP2, 2=BP1, 3=BP2

    moveStack stack ;
    stack.top=NULL ;

    // Black and White stacks are created with all pieces
    leftStack black, white ;
    black.top=NULL ;
    white.top=NULL ;

    display_name(p1Name, p2Name) ;

    createboard (&black, &white, board, empty, pieces) ;

    // If person chooses to stay in the game, stalemate condition is not achieved and pieces of both colors are present on the board
    while ( ans!=-1 && stalemateCtr<12 && remainingCtr(&black)>0 && remainingCtr(&white)>0 )
    {
        // Label for start of Player 1 (white)'s turn
        P1:
        system ( "cls" ) ;

        // Game board is displayed
        displayboard (board) ;

        // Initial scores of both teams are 0
        scoreboard (&BPoints, &WPoints, p1Name, p2Name) ;

        printStr2 ( "\n", 1, "\t", 9 ) ;
        printf ( "%s (W)'s turn ", p1Name ) ;
        printStr3 ( "\n", 1, "\t", 9, "-", 19 ) ;
        printStr2 ( "\n", 1, "\t", 9 ) ;
        printf ( "Which piece to move? (Enter -1 to exit) \n" ) ;

        // All white pieces in the left stack are displayed
        displayPiecesP1 (pieces) ;

        // Text box to enter the piece number is displayed
        display_textbox () ;

        //The cursor appears at the entered coordinates
        gotoxy(82,29) ;
        scanf ( "%d", &ans ) ;

        // Actions to take based on response
        switch ( ans )
        {
            case -1:

                // When the player wants to forfeit
                printStr2 ( "\n", 2, "\t", 8 ) ;
                printf( "Are you sure you want to forfeit? (Enter 1 for Yes And 2 for No): " ) ;
                scanf ( "%d", &forfeit ) ;

                if ( forfeit==1 )
                {
                    system("cls") ;
                    printStr2 ( "\n", 9, "\t", 9 ) ;
                    printf ( "%s wins! \n\n\n\n", p2Name ) ;
                    exit(0) ;
                }

                else
                    goto P1 ;
                break ;

            case 1:

                // Chosen piece is not present on the board
                if ( !(pieceExists( &white, pieces[0].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;
                    getch () ;
                    goto P1 ;
                }

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[0].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                // Valid moves for the piece are displayed
                movesExist = showvalidmovesforuser ( &stack, pieces[0],1,board,empty) ;

                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    // If user enters valid move
                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        // If pawn reaches opposite end of the board
                        if ( r==0 && promotedOrNot[0]==0 )
                        {
                            printStr2 ( "\n", 2, "\t", 9 ) ;
                            printf ( "Your pawn has been promoted to a rook! \n" ) ;
                            promote ( &pieces[0], &white,pieces) ;
                            promotedOrNot[0]=1 ;
                        }

                        // Points are recorded
                        scorecounter ( r, c,board, &BPoints, &WPoints ) ;
                        movepiece ( 0, r, c, &black, &stalemateCtr, board, pieces, empty ) ;
                    }

                    // If user plays an invalid move
                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move ** \n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;

                        emptyMoveStack ( &stack ) ;

                        getch() ;
                        goto P1 ;
                    }
                }

                // If no move is possible for the player,turn is skipped
                else
                {
                    if ( ( skipTurn(&stack,&white,1,pieces,board,empty) )==4 )
                        goto P2 ;

                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **. \n" ) ;

                    emptyMoveStack ( &stack ) ;

                    getch () ;
                    goto P1 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "**Press any key to continue**. \n" ) ;

                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            case 2:

                if ( !(pieceExists( &white, pieces[1].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **." ) ;
                    getch () ;
                    goto P1 ;
                }

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[1].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                movesExist = showvalidmovesforuser ( &stack, pieces[1], 1, board, empty ) ;

                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        if ( pieceExists(&white, pieces[1].name) )
                        {
                        scorecounter ( r, c, board, &BPoints, &WPoints) ;
                        movepiece ( 1, r, c, &black, &stalemateCtr, board, pieces, empty ) ;
                        }
                         else
                        {
                            printStr2 ( "\n", 2, "\t", 9 ) ;
                            printf ( "You can't move that piece! \n" ) ;
                            printStr2 ( "\n", 1, "\t", 9 ) ;
                            printf ( "**Press any key to continue**.\n" ) ;
                            getch () ;
                            goto P1 ;
                        }
                    }
                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move ** \n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;

                        emptyMoveStack ( &stack ) ;
                        getch() ;
                        goto P1 ;
                    }
                }

                else
                {
                    if ( (skipTurn(&stack,&white,1,pieces,board,empty))==4 )
                        goto P2 ;
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;

                    emptyMoveStack ( &stack ) ;
                    getch() ;
                    goto P1 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **.\n" ) ;

                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            case 3:

                if ( !(pieceExists( &white, pieces[2].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **." ) ;
                    getch () ;
                    goto P1 ;
                }

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[2].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                movesExist = showvalidmovesforuser ( &stack, pieces[2],1,board,empty) ;

                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        scorecounter ( r, c, board, &BPoints, &WPoints ) ;
                        movepiece ( 2, r, c, &black,&stalemateCtr,board,pieces,empty) ;
                    }
                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move **\n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;

                        emptyMoveStack ( &stack ) ;
                        getch() ;
                        goto P1 ;
                    }
                }

                else
                {
                    if ( (skipTurn(&stack,&white,1,pieces,board,empty))==4 )
                        goto P2 ;
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;
                    emptyMoveStack ( &stack ) ;
                    getch() ;
                    goto P1 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **.\n" ) ;

                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            case 4:

                if ( !(pieceExists( &white, pieces[3].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **." ) ;
                    getch () ;
                    goto P1 ;
                }

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[3].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                movesExist = showvalidmovesforuser ( &stack, pieces[3],1,board,empty) ;

                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        if ( r==0 && promotedOrNot[1]==0 )
                        {
                            printStr2 ( "\n", 2, "\t", 9 ) ;
                            printf ( "Your pawn has been promoted to a rook! \n" ) ;
                            promote ( &pieces[3], &white,pieces) ;
                            promotedOrNot[1]=1 ;
                        }
                        scorecounter ( r, c, board, &BPoints, &WPoints ) ;
                        movepiece ( 3, r, c, &black,&stalemateCtr,board,pieces,empty) ;
                    }

                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move ** \n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;
                        emptyMoveStack ( &stack ) ;
                        getch() ;
                        goto P1 ;
                    }
                }

                else
                {
                    if ( (skipTurn(&stack,&white,1,pieces,board,empty))==4 )
                        goto P2 ;
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;

                    emptyMoveStack ( &stack ) ;
                    getch() ;
                    goto P1 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **.\n" ) ;

                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            default:

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "** Invalid choice ** \n" ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **. " ) ;

                getch () ;
                goto P1 ;
        }

        if ( remainingCtr(&black)==0 )
            break ;

        // Label to mark starting of Player 2 (Black)'s turn
        P2:

        system ( "cls" ) ;

        displayboard (board) ;

        scoreboard (&BPoints,&WPoints, p1Name, p2Name) ;

        printStr2 ( "\n", 1, "\t", 9 ) ;
        printf ( "%s (B)'s turn ", p2Name ) ;
        printStr3 ( "\n", 1, "\t", 9, "-", 19 ) ;
        printStr2 ( "\n", 1, "\t", 9 ) ;
        printf ( "Which piece to move? (Enter -1 to exit) \n" ) ;

        displayPiecesP2 (pieces) ;

        display_textbox () ;

        gotoxy(82,29) ;
        scanf ( "%d", &ans ) ;

        switch ( ans )
        {

            case -1:

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf("Are you sure you want to forfeit? (Enter 1 for Yes And 2 for No): ");
                scanf("%d",&forfeit);

                if ( forfeit==1 )
                {
                    system ( "cls" ) ;
                    printStr2 ( "\n", 9, "\t", 9 ) ;
                    printf ( "%s wins! \n\n\n\n", p1Name ) ;
                    exit(0) ;
                }

                else
                    goto P2 ;
                break ;

            case 1:

                if ( !(pieceExists( &black, pieces[4].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "**Press any key to continue**." ) ;
                    getch () ;
                    goto P2 ;
                }

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[4].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                movesExist = showvalidmovesforuser ( &stack, pieces[4],1,board,empty) ;

                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        if ( r==3 && promotedOrNot[2]==0 )
                        {
                            printStr2 ( "\n", 2, "\t", 9 ) ;
                            printf ( "Your pawn has been promoted to a rook! \n" ) ;
                            promote ( &pieces[4], &black, pieces ) ;
                            promotedOrNot[2]=1 ;
                        }
                        scorecounter ( r, c, board, &BPoints, &WPoints ) ;
                        movepiece ( 4, r, c, &white, &stalemateCtr, board, pieces, empty ) ;
                    }
                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move ** \n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;

                        emptyMoveStack ( &stack ) ;
                        getch() ;
                        goto P2 ;
                    }
                }
                else
                {
                    if ( (skipTurn(&stack,&black,0,pieces,board,empty))==4 )
                        goto P1 ;

                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;

                    emptyMoveStack ( &stack ) ;
                    getch() ;
                    goto P2 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "**Press any key to continue**.\n" ) ;
                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            case 2:

                if ( !(pieceExists( &black, pieces[5].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **." ) ;
                    getch () ;
                    goto P2 ;
                }

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[5].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                movesExist = showvalidmovesforuser ( &stack, pieces[5], 1,board,empty) ;

                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        if ( pieceExists(&black, pieces[5].name) )
                        {
                            scorecounter ( r, c,board,&BPoints,&WPoints) ;
                            movepiece ( 5, r, c, &white,&stalemateCtr,board,pieces,empty) ;
                        }
                        else
                        {
                            printStr2 ( "\n", 2, "\t", 9 ) ;
                            printf ( "You can't move that piece! \n" ) ;
                            printStr2 ( "\n", 1, "\t", 9 ) ;
                            printf ( "**Press any key to continue**.\n" ) ;
                            getch () ;
                            goto P2 ;
                        }
                    }
                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move ** \n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;

                        emptyMoveStack ( &stack ) ;
                        getch() ;
                        goto P2 ;
                    }
                }
                else
                {
                    if ( (skipTurn(&stack,&black,0,pieces,board,empty))==4 )
                        goto P1 ;
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;

                    emptyMoveStack ( &stack ) ;
                    getch() ;
                    goto P2 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **.\n" ) ;

                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            case 3:

                if ( !(pieceExists( &black, pieces[6].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **." ) ;
                    getch () ;
                    goto P2 ;
                }


                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[6].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                movesExist = showvalidmovesforuser ( &stack, pieces[6],1,board,empty) ;
                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        scorecounter ( r, c,board,&BPoints,&WPoints) ;
                        movepiece ( 6, r, c, &white,&stalemateCtr,board,pieces,empty) ;
                    }
                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move ** \n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;

                        emptyMoveStack ( &stack ) ;
                        getch() ;
                        goto P2 ;
                    }
                }
                else
                {
                    if ( (skipTurn(&stack,&black,0,pieces,board,empty))==4 )
                        goto P1 ;
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;

                    emptyMoveStack ( &stack ) ;
                    getch() ;
                    goto P2 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **.\n" ) ;

                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            case 4:

                if ( !(pieceExists( &black, pieces[7].name )) )
                {
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "You can't move that piece! \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **." ) ;
                    getch () ;
                    goto P2 ;
                }

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "Chosen piece: %s. ", pieces[7].name ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "Possible moves: \n" ) ;

                movesExist = showvalidmovesforuser ( &stack, pieces[7],1,board,empty) ;

                if ( movesExist==1 )
                {
                    printStr ( "\t", 9 ) ;
                    printf ( "R: " ) ;
                    scanf ( "%d", &r ) ;
                    printStr ( "\t", 9 ) ;
                    printf ( "C: " ) ;
                    scanf ( "%d", &c ) ;

                    if ( checkIfValidMove( &stack, r, c ) )
                    {
                        if ( r==3 && promotedOrNot[3]==0 )
                        {
                            printStr2 ( "\n", 2, "\t", 9 ) ;
                            printf ( "Your pawn has been promoted to a rook! \n" ) ;
                            promote ( &pieces[7], &black,pieces) ;
                            promotedOrNot[3]=1 ;
                        }
                        scorecounter ( r, c,board,&BPoints,&WPoints) ;
                        movepiece ( 7, r, c, &white,&stalemateCtr,board,pieces,empty) ;
                    }
                    else
                    {
                        printStr2 ( "\n", 2, "\t", 9 ) ;
                        printf ( "** Invalid move ** \n" ) ;
                        printStr2 ( "\n", 1, "\t", 9 ) ;
                        printf ( "** Press any key to continue **.\n" ) ;
                        emptyMoveStack ( &stack ) ;
                        getch() ;
                        goto P2 ;
                    }
                }
                else
                {
                    if ( (skipTurn(&stack,&black,0,pieces,board,empty))==4 )
                        goto P1 ;
                    printStr2 ( "\n", 2, "\t", 9 ) ;
                    printf ( "No move available. \n" ) ;
                    printStr2 ( "\n", 1, "\t", 9 ) ;
                    printf ( "** Press any key to continue **.\n" ) ;

                    emptyMoveStack ( &stack ) ;
                    getch() ;
                    goto P2 ;
                }

                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **.\n" ) ;

                emptyMoveStack ( &stack ) ;
                getch() ;
                break ;

            default:

                printStr2 ( "\n", 2, "\t", 9 ) ;
                printf ( "** Invalid choice ** \n" ) ;
                printStr2 ( "\n", 1, "\t", 9 ) ;
                printf ( "** Press any key to continue **. " ) ;
                getch () ;
                goto P2 ;
        }
        system ( "cls" ) ;
    }
    if ( stalemateCtr<12 )
    {
        system ( "cls" ) ;
        declareWinner ( &black, &white, &WPoints, &BPoints, p1Name, p2Name ) ;
        return 0 ;
    }
    else if ( stalemateCtr>=12 && (remainingCtr(&black)==1 || remainingCtr(&white)==1) )
    {
        system ( "cls" ) ;
        printStr2 ( "\n", 2, "\t", 9 ) ;
        printf ( "**** Stalemate Condition ****\n\n\n\n" ) ;
        declareWinner(&black, &white, &WPoints, &BPoints, p1Name, p2Name) ;
    }
    else
        goto P1 ;
    return 0 ;
}







