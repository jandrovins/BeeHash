SRC=src
INCLUDE=src/include
BIN=bin
OBJ=$(BIN)/obj
CXXFLAGS=-Ofast
CXX=g++
MKDIR=mkdir -p
MV=mv

all: $(BIN)/beehash_all_times $(BIN)/beehash_single_dataset_times $(BIN)/beehash

$(BIN)/beehash_all_times: $(SRC)/beehash_all_times.cpp
	$(MKDIR) $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)/beehash_single_dataset_times: $(SRC)/beehash_single_dataset_times.cpp
	$(MKDIR) $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(BIN)
