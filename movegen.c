#include "stdio.h"
#include "defs.h"

#define MOVE(f, t, ca, pro, fl)  ((f)  | ((t) <<7) | ((ca) << 14) | ((pro) << 20)  | (fl) )
#define SQOFFBOARD(sq) (FilesBoard[sq] == OFF_BOARD)

const int LoopSlidePce[8] = {
    wB, wR, wQ, 0, bB, bR, bQ, 0
};

const int LoopNonSlidePce[6] = {
    wN, wK, 0, bN, bK, 0
};

const int LoopSlideIndex[2] = {0, 4};
const int LoopNonSlideIndex[2] = {0,3};

const int PceDir[13][8] = { 
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {-8,-19,-21,-12,8,19,21,12},
    {-9,-11,11,9,0,0,0,0},
    {-1,-10,1,10,0,0,0,0},
    {-1,-10,1,10,-9,-11,11,9},
    {-1,-10,1,10,-9,-11,11,9},
    {0,0,0,0,0,0,0,0},
    {-8,-19,-21,-12,8,19,21,12},
    {-9,-11,11,9,0,0,0,0},
    {-1,-10,1,10,0,0,0,0},
    {-1,-10,1,10,-9,-11,11,9},
    {-1,-10,1,10,-9,-11,11,9}
};

int NumDir[13] = {
    0,0,8,4,4,8,8,0,8,4,4,8,8
};

const int VictimScore[13] = {0, 100, 200, 300, 400, 500, 600, 100, 200 ,300, 400, 500, 600};
static int MvvLvaScores[13][13];

void InitMvvLva() {
	int Attacker;
	int Victim;
	for(Attacker = wP; Attacker <= bK; ++Attacker) {
		for(Victim = wP; Victim <= bK; ++Victim) {
			MvvLvaScores[Victim][Attacker] = VictimScore[Victim] + 6 - ( VictimScore[Attacker] / 100);
		}
	}
}

static void AddQuietMove(const S_BOARD * pos, int move, S_MOVELIST * list) { 
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

static void AddCaptureMove(const S_BOARD * pos, int move, S_MOVELIST * list) { 
    list->moves[list->count].move = move;
    list->moves[list->count].score = MvvLvaScores[CAPTURED(move)][pos->pieces[FROMSQ(move)]];
    list->count++;
}

static void AddEnPassantMove(const S_BOARD * pos, int move, S_MOVELIST * list) { 
    list->moves[list->count].move = move;
    list->moves[list->count].score = 105;
    list->count++;
}

static void AddWhitePawnCaptureMove(const S_BOARD * pos, const int from, const int to, const int cap, S_MOVELIST * list) { 
    if(RanksBoard[from] == RANK_7) { 
        
        ASSERT(PieceValidEmpty(cap));
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));
        AddCaptureMove(pos, MOVE(from, to, cap,wQ ,0), list);
        AddCaptureMove(pos, MOVE(from, to, cap,wN ,0), list);
        AddCaptureMove(pos, MOVE(from, to, cap,wB ,0), list);
        AddCaptureMove(pos, MOVE(from, to, cap,wR ,0), list);
    } else { 
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }

}

static void AddBlackPawnCaptureMove(const S_BOARD * pos, const int from, const int to, const int cap, S_MOVELIST * list) { 
    if(RanksBoard[from] == RANK_2) {
        ASSERT(PieceValidEmpty(cap));
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));
        

        AddCaptureMove(pos, MOVE(from, to, cap,bQ ,0), list);
        AddCaptureMove(pos, MOVE(from, to, cap,bN ,0), list);
        AddCaptureMove(pos, MOVE(from, to, cap,bB ,0), list);
        AddCaptureMove(pos, MOVE(from, to, cap,bR ,0), list);
    } else { 
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }

}

static void AddWhitePawnMove(const S_BOARD * pos, const int from, const int to, S_MOVELIST * list) { 
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to)); 
    if(RanksBoard[from] == RANK_7) { 

        AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);  
        AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);  
        AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);  
        AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);  
    } else { 
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);  
    }
}

static void AddBlackPawnMove(const S_BOARD * pos, const int from, const int to, S_MOVELIST * list) { 
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(RanksBoard[from] == RANK_2) { 
        AddQuietMove(pos, MOVE(from, to, EMPTY, bQ, 0), list);  
        AddQuietMove(pos, MOVE(from, to, EMPTY, bB, 0), list);  
        AddQuietMove(pos, MOVE(from, to, EMPTY, bN, 0), list);  
        AddQuietMove(pos, MOVE(from, to, EMPTY, bR, 0), list);  
    } else { 
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);  
    }
}

void GenerateAllMoves(const S_BOARD * pos , S_MOVELIST * list) { 
    ASSERT(check_board(pos));

    list->count = 0;
    int pce = EMPTY;
    int side = pos->side;
    int sq = 0; int t_sq = 0;
    int pceNum = 0;
    int pceIndex = 0;
    int index = 0;

    if(side == WHITE) { 
        for(pceNum = 0; pceNum < pos->pceNum[wP]; pceNum++) { 
            sq = pos->pList[wP][pceNum];
            ASSERT(SqOnBoard(sq));
            if(pos->pieces[sq + 10] == EMPTY) { 
                AddWhitePawnMove(pos, sq, sq+10, list);
                if(RanksBoard[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY) { 
                    AddQuietMove(pos, MOVE(sq, (sq+20), EMPTY, EMPTY, MFLAGPS), list);
                }

            }

			if(!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK) {
				AddWhitePawnCaptureMove(pos, sq, sq+9, pos->pieces[sq + 9], list);
			}
			if(!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK) {
				AddWhitePawnCaptureMove(pos, sq, sq+11, pos->pieces[sq + 11], list);
			}

			if(pos->enPas != NO_SQ) {
				if(sq + 9 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq + 9,EMPTY,EMPTY,MFLAGEP), list);
				}
				if(sq + 11 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq + 11,EMPTY,EMPTY,MFLAGEP), list);
				}
			}

        }
        if(pos->castlePerm & WKCA) {
            if(pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY) {
                if(!SqAttacked(E1,BLACK,pos) && !SqAttacked(F1,BLACK,pos) ) {
                    AddQuietMove(pos, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }
        
        if(pos->castlePerm & WQCA) { 
            if(pos->pieces[B1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[D1] == EMPTY){
                if(!SqAttacked(E1, BLACK, pos) && !SqAttacked(D1, BLACK, pos)) {

                    AddQuietMove(pos, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA ), list);
                }
            }
        }
    } else { //black moves 

        for(pceNum = 0; pceNum < pos->pceNum[bP]; pceNum++) { 
            sq = pos->pList[bP][pceNum];
            ASSERT(SqOnBoard(sq));

            if(pos->pieces[sq - 10] == EMPTY) { 
                AddBlackPawnMove(pos, sq, sq-10, list);
                if(RanksBoard[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY) { 
                    AddQuietMove(pos, MOVE(sq, (sq-20), EMPTY, EMPTY, MFLAGPS), list);
                }

            } 

            if(!SQOFFBOARD(sq - 9) && PieceCol[pos->pieces[sq - 9]] == WHITE) {
				AddBlackPawnCaptureMove(pos, sq, sq-9, pos->pieces[sq - 9], list);
			}
			if(!SQOFFBOARD(sq - 11) && PieceCol[pos->pieces[sq - 11]] == WHITE) {
				AddBlackPawnCaptureMove(pos, sq, sq-11, pos->pieces[sq - 11], list);
			}

			if(pos->enPas != NO_SQ) {
				if(sq - 9 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq - 9,EMPTY,EMPTY,MFLAGEP), list);
				}
				if(sq - 11 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq - 11,EMPTY,EMPTY,MFLAGEP), list);
				}
			}

        }
        if(pos->castlePerm &  BKCA) {
            if(pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY) {
                if(!SqAttacked(E8,WHITE,pos) && !SqAttacked(F8,WHITE,pos) ) {
                    AddQuietMove(pos, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }

        if(pos->castlePerm &  BQCA) {
            if(pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY) {
                if(!SqAttacked(E8,WHITE,pos) && !SqAttacked(D8,WHITE,pos) ) {
                    AddQuietMove(pos, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }
    }
    
    //generate for sliding pieces
    pceIndex = LoopSlideIndex[side];
    pce = LoopSlidePce[pceIndex++];
    while(pce != 0) { 
        ASSERT(PieceValid(pce));
        
        for(pceNum = 0; pceNum <pos->pceNum[pce]; pceNum++) { 
            sq = pos->pList[pce][pceNum];
            ASSERT(SqOnBoard(sq));

            for(index = 0; index < NumDir[pce]; index++) {
                int dir = PceDir[pce][index];
                t_sq = sq + dir;

                while(!SQOFFBOARD(t_sq)){ 
                    if(pos->pieces[t_sq] != EMPTY) { 
                        if( PieceCol[pos->pieces[t_sq]] == (side^ 1)) { 
                            AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                        }
                        break;
                    }
                    AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
                    t_sq = t_sq + dir;
                }

            }

        }

        pce = LoopSlidePce[pceIndex++];
        
    }

    //generate moves for non sliding pieces
    pceIndex = LoopNonSlideIndex[side];
    pce = LoopNonSlidePce[pceIndex++];
    while(pce != 0) { 
        ASSERT(PieceValid(pce));

        for(pceNum = 0; pceNum < pos->pceNum[pce]; pceNum++) { 
            sq = pos->pList[pce][pceNum];
            ASSERT(SqOnBoard(sq));
            
            for(index = 0; index < NumDir[pce]; index++) {
                int dir = PceDir[pce][index];
                t_sq = sq + dir;
                
                if(SQOFFBOARD(t_sq)){ 
                    continue;
                }

                if(pos->pieces[t_sq] != EMPTY) { 
                    if(PieceCol[pos->pieces[t_sq]] == (side ^1)) { 
                        AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                    }
                    continue;
                }

                AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);

            }
        }

        pce = LoopNonSlidePce[pceIndex++];
    }


}

int MoveExists(S_BOARD * pos, const int move) { 
    S_MOVELIST list[1];
    
    GenerateAllMoves(pos, list);

    for(int moveNum = 0; moveNum < list->count; moveNum++) { 
        if(!MakeMove(pos, list->moves[moveNum].move)) { 
            continue;
        }
        TakeMove(pos);
        
        if(list->moves[moveNum].move == move) { 
            return TRUE;
        }
    }
    return FALSE;
}