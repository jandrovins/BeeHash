SRC=src
INCLUDE=src/include
BIN=bin
OBJ=$(BIN)/obj
CXXFLAGS=-Ofast
CXX=g++

all: $(BIN)/spatialhashing $(BIN)/spatialhashing_times

$(BIN)/spatialhashing: $(SRC)/spatialhashing.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)/spatialhashing_times: $(SRC)/spatialhashing_times.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(BIN)
