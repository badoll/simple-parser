WALL = -Wall
CPP11 = -std=c++11
SRC = .
BIN = ./bin
OBJ = ./obj
MAIN = $(BIN)/main
OBJECT = $(OBJ)/main.o $(OBJ)/word.o  \
		$(OBJ)/lexical_parser.o $(OBJ)/syntactic_parser.o
		
HEADER = $(SRC)/word.h $(SRC)/lexical_parser.h $(SRC)/syntactic_parser.h 

.PHONY: all
all: main

main: $(OBJECT)
	g++ $^ -o $(MAIN) $(WALL) $(CPP11)


$(OBJECT): $(HEADER)

$(OBJ)/%.o: $(SRC)/%.cpp
	g++ -c $< -o $@ $(WALL) $(CPP11)

.PHONY: clean
clean:
	rm -f $(MAIN)
	rm -f $(OBJ)/*.o