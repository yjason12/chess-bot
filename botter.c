#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define PERFT "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define queenFEN "8/3q4/8/8/4Q3/8/8/8 w - - 0 2"
#define PERFTFEN "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"

#define WHITEPAWNMOVES "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define BLACKPAWNMOVES "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 b - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 w - - 0 1"
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"

#define CASTLE2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define WAC1 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"

void ShowAttack(const int side, const S_BOARD * pos) { 
    int rank = 0;
    int file = 0;
    int sq = 0;

    printf("\n\nSqares attacked by: %c\n", SideChar[side]);

    for(rank = RANK_8; rank >= RANK_1; rank--) { 
        for(file = FILE_A; file <= FILE_H; file++) { 
            sq = FR2SQ(file, rank);
            if(SqAttacked(sq, side, pos) == TRUE) { 
                printf("X");
            } else { 
                printf("-");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}


int main()
{

    AllInit();//here
    S_BOARD board[1];
    S_MOVELIST list[1];
    S_SEARCHINFO info[1];
    parse_fen(WAC1, board);
    // PerftTest(4, board); 

    char input[6];
    int move = NOMOVE;
    int PvNum = 0;
    int Max = 0;

    while(TRUE) { 
        print_board(board);
        printf("\nPlease enter a move > ");
        fgets(input, 6, stdin);

        fflush(stdin);

        if(input[0] == 'q') { 
            break;
        } else if(input[0] == 't') { 
            TakeMove(board);
            continue;
        } else if (input[0] == 's') { 
            
            info->depth = 4;
            SearchPosition(board, info);
        } else { 
            move = ParseMove(input, board);
            if(move != NOMOVE) { 
                MakeMove(board, move);

            } else {
                printf("Move Not Parsed: %s\n", input);
            }
        }

        fflush(stdin);
    }

    return 0;    
}


