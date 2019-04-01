all:
	gcc -Wall -c helper.c -std=c11
	gcc -Wall -c board.c -std=c11
	gcc -Wall -o four main.c helper.o board.o -lncurses -lm -std=c11

clean:
	rm -r four
	rm -r helper.o
	rm -r board.o
