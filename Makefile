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


$(EXEC): $(OBJ_DIR)/main.o
	g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.cpp
	g++ -c $(CPPFLAGS) $< -o $@
