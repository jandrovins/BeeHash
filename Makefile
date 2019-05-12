SRC=src
INCLUDE=src/include
BIN=bin
OBJ=$(BIN)/obj
CXXFLAGS=-fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline -fno-inline -O2 -g
CXX=g++

all: $(BIN)/spatialhashing #$(BIN)/robin_map_spatialhashing 

$(BIN)/spatialhashing: $(SRC)/spatialhashing.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(BIN)
