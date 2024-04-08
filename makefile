CC = clang++
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
BIN = bin/main.exe

all : $(BIN)

$(BIN) : $(OBJ)
	$(CC) $(OBJ) -o $@

obj/%.o : src/%.cpp
	$(CC) -c $< -o $@

run : $(BIN)
	.\\$(BIN)

clean :
	rm obj/* bin/*
