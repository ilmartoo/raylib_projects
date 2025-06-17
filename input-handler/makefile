CC       := gcc
CC_FLAGS := -Wall -g

OUTPUT := input-handler.exe

SOURCES        := main.c input-handler.c
SOURCES_FOLDER := src

INCLUDE_FOLDERS := src

LIB_FOLDERS :=
LIBS := raylib opengl32 gdi32 winmm m

.PHONY: build

build:
	$(CC) $(CC_FLAGS) $(addprefix $(SOURCES_FOLDER)/,$(SOURCES)) -o $(OUTPUT) $(addprefix -I,$(INCLUDE_FOLDERS)) $(addprefix -l,$(LIBS)) $(addprefix -L,$(LIB_FOLDERS)) 