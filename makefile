all: p06
p06: removal main.o
	g++ -o p06 main.o
main.o: main.cpp
	g++ -c -g main.cpp
removal:
	rm -f *.o