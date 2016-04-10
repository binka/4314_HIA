BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
EXEC = $(BIN_DIR)/HIA
CPPFLAGS = -Wall -std=c++11 -O3 
#-fopenmp
LDFLAGS = 
#-fopenmp



.PHONY: clean_out

all: clean $(EXEC)

install:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	make all

clean:
	/bin/rm -f $(BIN_DIR)/*
	/bin/rm -f $(OBJ_DIR)/*


$(EXEC): $(OBJ_DIR)/main.o $(OBJ_DIR)/HybridIndex.o $(OBJ_DIR)/GappedSequence.o $(OBJ_DIR)/FastaFile.o $(OBJ_DIR)/Aligner.o
	g++ -o $@ $^ $(LDFLAGS) $(SRC_DIR)/HybridIndex.h $(SRC_DIR)/GappedSequence.h $(SRC_DIR)/FastaFile.h $(SRC_DIR)/Aligner.h

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -c $(CPPFLAGS) $< -o $@
