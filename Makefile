CXX = g++
CC = gcc

CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp

OUT = app

SRC_CPP = $(wildcard src/*.cpp)
SRC_C = $(wildcard src/*.c)

OBJ = $(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o)

all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) $(OBJ) -o $(OUT) $(LIBS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ -Iinclude

clean:
	rm -f src/*.o $(OUT)

run: all
	./$(OUT)	

