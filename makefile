all:
	gcc evaluate.c pvtable.c misc.c io.c perft.c search.c botter.c makemove.c movegen.c validate.c init.c bitboards.c hashkeys.c board.c data.c attack.c -o botter -Wall -Wextra -g
