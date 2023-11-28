# PATH_BIN     := bin
PATH_SRC 	 := src
OUT_EXE      = a.out

CXX          = g++
CXXFLAGS     = -std=c++11
CC           = $(CXX) $(CXXFLAGS)

a.out: main.o util.o
	$(CC) main.o util.o

main.o: $(PATH_SRC)/main.cpp $(PATH_SRC)/util.hpp
	$(CC) -c $(PATH_SRC)/main.cpp -o main.o

util.o: $(PATH_SRC)/util.cpp $(PATH_SRC)/util.hpp
	$(CC) -c $(PATH_SRC)/util.cpp -o util.o

clean:
	$(RM) $(PATH_BIN) *.o $(OUT_EXE)