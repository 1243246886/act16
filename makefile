CFLAGS=-g -Wall
CXXFLAGS=$(CFLAGS)

main: main.o dfa.o flowgraph.o tokens.o parser.o
	g++ -o main $^ -lfl

parser.cpp: three-addr.y
	bison -d -o $@ $^

parser.hpp: parser.cpp

parser.o: parser.cpp

tokens.cpp: three-addr.l parser.hpp
	flex -o $@ $^

.PHONY: clean
clean:
	rm -f tokens.* parser.* *.o
