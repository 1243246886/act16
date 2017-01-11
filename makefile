parser.cpp: three-addr.y
	bison -d -o $@ $^

parser.hpp: parser.cpp

parser.o: parser.cpp

tokens.cpp: three-addr.l parser.hpp
	flex -o $@ $^
