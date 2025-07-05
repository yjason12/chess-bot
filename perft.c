#include "defs.h"
#include "stdio.h"

long leafNodes;

void Perft(int depth, S_BOARD * pos ) { 
    ASSERT(check_board(pos));

    if (depth == 0) { 
        leafNodes++;
        return;
    }

    S_MOVELIST list[1];

    GenerateAllMoves(pos, list);

    int MoveNum = 0;
    for(MoveNum = 0; MoveNum < list->count; MoveNum++) { 
        
        if(!MakeMove(pos, list->moves[MoveNum].move)) { 
            continue;
        }
        
        Perft(depth - 1, pos);
        TakeMove(pos);
    }
}

void PerftTest(int depth, S_BOARD * pos ) { 
    ASSERT(check_board(pos));

    if (depth == 0) { 
        leafNodes++;
        return;
    }
    
    printf("\nStarting Test to Depth: %d\n", depth);
    
    int start = GetTimeMs();

    S_MOVELIST list[1];

    GenerateAllMoves(pos, list);

    int MoveNum = 0;
    for(MoveNum = 0; MoveNum < list->count; MoveNum++) { 
        int move = list->moves[MoveNum].move;
        if(!MakeMove(pos, move)) { 
            continue;
        }
        long totalNodes = leafNodes;
        Perft(depth - 1, pos);
        TakeMove(pos);

        long oldNodes = leafNodes - totalNodes;

        printf("move %d: %s : %ld\n", MoveNum+1, PMove(move), oldNodes);
    }

    printf("\nTest Complete: %ld nodes visited in %dms\n", leafNodes, GetTimeMs() - start);
}



