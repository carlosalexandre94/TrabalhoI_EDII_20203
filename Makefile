all: main.o 
	g++ -o main main.o
	rm -rf *.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp
