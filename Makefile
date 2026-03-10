GXX = g++
FLAGS = -Wall -Werror -Wextra -Wpedantic -std=c++23
SRC = src/main.cpp src/models/src/entry.cpp src/models/src/user.cpp
OUT = hobbit

all: 
	$(GXX) $(FLAGS) $(SRC) -o $(OUT)
