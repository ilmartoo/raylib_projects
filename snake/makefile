CC       := gcc
CC_FLAGS := -Wall -g

OUTPUT  := snake.exe
SOURCES    := src/*.c
INCLUDES    := -I.
LIBS   := -lraylib -lopengl32 -lgdi32 -lwinmm -lm

.PHONY: build

build:
	$(CC) $(CC_FLAGS) $(SOURCES) -o $(OUTPUT) $(INCLUDES) $(LIBS)
