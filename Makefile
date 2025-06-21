CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -Iinclude
SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = binwatch

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) log.txt

.PHONY: all clean