#Makefile 

map: map.cpp
	gcc -c -Wall -Werror heap.c -o heap.o
	g++ *.cpp heap.o -o map -Wall -Werror -lncurses -g


clean:
	rm -f map *~ *.o core
