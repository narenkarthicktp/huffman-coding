CC = clang++
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
BIN = bin/main.exe

all : $(BIN)

$(BIN) : $(OBJ)
	$(CC) $(OBJ) -g -o $@

obj/%.o : src/%.cpp
	$(CC) -c $< -g -o $@

run : $(BIN)
	.\\$(BIN)

clean :
	rm obj/* bin/*
