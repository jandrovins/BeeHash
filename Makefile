SRC=src
INCLUDE=src/include
BIN=bin
OBJ=$(BIN)/obj
CXXFLAGS=-O3 -fopenmp
CXX=g++

all:  $(BIN)/spatialhashing

$(BIN)/spatialhashing: $(SRC)/spatialhashing.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(BIN)
