#include "stdio.h"
#include "defs.h"

char * PrSq(const int sq) { 
    static char SqStr[3];
    int file = FilesBoard[sq];
    int rank = RanksBoard[sq];

    sprintf(SqStr, "%c%c", ('a' + file), ('1' + rank));
    
    return SqStr;
    
}

char * PMove(const int move) {

	static char MvStr[6];

	int ff = FilesBoard[FROMSQ(move)];
	int rf = RanksBoard[FROMSQ(move)];
	int ft = FilesBoard[TOSQ(move)];
	int rt = RanksBoard[TOSQ(move)];

	int promoted = PROMOTED(move);

	if(promoted) {
		char pchar = 'q';
		if(IsKn(promoted)) {
			pchar = 'n';
		} else if(IsRQ(promoted) && !IsBQ(promoted))  {
			pchar = 'r';
		} else if(!IsRQ(promoted) && IsBQ(promoted))  {
			pchar = 'b';
		}
		sprintf(MvStr, "%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pchar);
	} else {
		sprintf(MvStr, "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
	}

	return MvStr;
}

char *PrMove(const int move) {

	static char MvStr[6];

	int ff = FilesBoard[FROMSQ(move)];
	int rf = RanksBoard[FROMSQ(move)];
	int ft = FilesBoard[TOSQ(move)];
	int rt = RanksBoard[TOSQ(move)];

	int promoted = PROMOTED(move);

	if(promoted) {
		char pchar = 'q';
		if(IsKn(promoted)) {
			pchar = 'n';
		} else if(IsRQ(promoted) && !IsBQ(promoted))  {
			pchar = 'r';
		} else if(!IsRQ(promoted) && IsBQ(promoted))  {
			pchar = 'b';
		}
		sprintf(MvStr, "%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pchar);
	} else {
		sprintf(MvStr, "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
	}

	return MvStr;
}

void PrintMoveList(const S_MOVELIST * list) { 
	int index = 0;
	int score = 0;
	int move = 0;

	printf("MoveList:\n");


	for(index = 0; index < list->count; index++) { 
		score = list->moves[index].score;
		move = list->moves[index].move;
		
		printf("Move:%d > %s (score:%d)\n", index + 1, PrMove(move), score);

	}

	printf("MoveList total moves: %d\n\n", list->count);	
}

int ParseMove(char * move, S_BOARD * pos) { 
	if(move[0] < 'a' || move[0] > 'h') return NOMOVE;
	if(move[1] < '1' || move[1] > '8') return NOMOVE;
	if(move[2] < 'a' || move[2] > 'h') return NOMOVE;
	if(move[3] < '1' || move[3] > '8') return NOMOVE;

	int from = FR2SQ(move[0] - 'a', move[1]-'1');
	int to = FR2SQ(move[2] - 'a', move[3]-'1');


	ASSERT(SqOnBoard(from) &&  SqOnBoard(to));

	S_MOVELIST list[1];
	GenerateAllMoves(pos, list);
	
	int moveNum = 0;
	int movet = 0;
	int pp = EMPTY;

	for(moveNum = 0; moveNum < list->count; moveNum++) { 
		movet = list->moves[moveNum].move;

		if(FROMSQ(movet) == from && TOSQ(movet) == to) { 
			pp = PROMOTED(movet);	

			if(pp != EMPTY) { 
				if(IsRQ(pp) && !IsBQ(pp) && move[4] == 'r') { 
					return movet;
				} else if (!IsRQ(pp) && IsBQ(pp) && move[4] == 'b') { 
					return movet;
				} else if(IsRQ(pp) && IsBQ(pp) && move[4] == 'q') { 
					return movet;
				} else if (IsKn(pp) && move[4] == 'n') { 
					return movet;
				}
				continue;
			}

			return movet;
		}
	}

	return NOMOVE;
}