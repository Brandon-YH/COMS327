#Makefile 

map: map.cpp pokemon.cpp data.cpp item.cpp
	gcc -c -Wall -Werror heap.c -o heap.o
	g++ *.cpp heap.o -o map -Wall -Werror -lncurses -g


clean:
	rm -f map *~ *.o core
