SRC=src
INCLUDE=src/include
BIN=bin
OBJ=$(BIN)/obj
CXXFLAGS= #-fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline -fno-inline -O2 -fopenmp
CXX=g++
TSL_LIBRARY_DIR=/home/vincent/SOURCE/robin-map/include/

all: $(BIN)/robin_map_spatialhashing #$(BIN)/spatialhashing

$(BIN)/spatialhashing: $(SRC)/spatialhashing.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)/robin_map_spatialhashing: $(SRC)/robin_map_spatialhashing.cpp
	$(CXX) $(CXXFLAGS) -I$(TSL_LIBRARY_DIR) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(BIN)
