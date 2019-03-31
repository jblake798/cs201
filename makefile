all:
	gcc -Wall -c helper.c
	gcc -Wall -c board.c
	gcc -Wall -o four main.c helper.o board.o -lncurses -lm

clean:
	rm -r four
	rm -r helper.o
	rm -r board.o
