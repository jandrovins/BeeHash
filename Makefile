SRC=src
INCLUDE=src/include
BIN=bin
OBJ=$(BIN)/obj
CXXFLAGS=-Ofast
CXX=g++
MKDIR=mkdir -p
MV=mv
CP=cp

all: $(BIN)/create_pcd $(BIN)/beehash $(BIN)/beehash_all_times $(BIN)/beehash_single_dataset_times $(BIN)/open3d_visualize

$(BIN)/beehash_all_times: $(SRC)/beehash_all_times.cpp
	$(MKDIR) $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)/beehash_single_dataset_times: $(SRC)/beehash_single_dataset_times.cpp
	$(MKDIR) $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)/beehash: $(SRC)/beehash.cpp
	$(MKDIR) $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)/create_pcd: $(SRC)/create_pcd.cpp
	$(MKDIR) $(BIN)
	$(CXX) -g -o $@ $^

$(BIN)/open3d_visualize: $(SRC)/open3d_visualize
	$(MKDIR) $(BIN)
	$(CP) $< $@


.PHONY: clean
clean:
	rm -rf $(BIN)
