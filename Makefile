CC = gcc

CFLAGS = -Wall -Wextra -std=c17 -Iinclude
LDFLAGS = `pkg-config --libs sdl2 gl`

T ?= debug

ifeq ($T,release)
CFLAGS += -O3 -DNDEBUG
else ifeq ($T,debug)
CFLAGS += -g -O0 -DDEBUG #-fsanitize=address,undefined
else
$(error invalid mode $T)
endif

SRC = src
SOURCE = $(wildcard $(SRC)/*.c)
OBJECT = $(SOURCE:$(SRC)/%.c=$T_%.o)

TARGET = $T.out

.PHONY: clean

all: $(TARGET)

$T_%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECT)
	$(CC) $(OBJECT) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	-rm *.o *.out
