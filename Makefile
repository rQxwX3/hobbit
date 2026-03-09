GXX = g++
FLAGS = -Wall -Werror -Wextra -Wpedantic -std=c++23
SRC = main.cpp
OUT = hobbit

all: 
	$(GXX) $(FLAGS) $(SRC) -o $(OUT)
