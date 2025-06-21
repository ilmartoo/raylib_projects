CC       := gcc
CC_FLAGS := -Wall -g

MIDDLEWARE_SRCS   := input-handler.c float16.c
MIDDLEWARE_FOLDER := src

BASE_OUTPUT  := input-handler.exe
BASE_SRCS    := device-tester.c
BASE_FOLDER  := src
BASE_INCLUDE := $(BASE_FOLDER) 

# TESTING_OUTPUT  := tests-$(BASE_OUTPUT)
# TESTING_SRCS    := tests.c
# TESTING_FOLDER  := test
# TESTING_INCLUDE := $(TESTING_FOLDER) 


LIB_FOLDERS :=
LIBS        := raylib opengl32 gdi32 winmm m

.PHONY: build

build:
	$(CC) $(CC_FLAGS) $(addprefix $(MIDDLEWARE_FOLDER)/,$(MIDDLEWARE_SRCS)) $(addprefix $(BASE_FOLDER)/,$(BASE_SRCS)) -o $(BASE_OUTPUT) $(addprefix -I,$(BASE_INCLUDE)) $(addprefix -l,$(LIBS)) $(addprefix -L,$(LIB_FOLDERS))

# tests:
# 	$(CC) $(CC_FLAGS) $(addprefix $(MIDDLEWARE_FOLDER)/,$(MIDDLEWARE_SRCS)) $(addprefix $(TESTING_FOLDER)/,$(TESTING_SRCS)) -o $(TESTING_OUTPUT) $(addprefix -I,$(TESTING_INCLUDE)) $(addprefix -l,$(LIBS)) $(addprefix -L,$(LIB_FOLDERS))
# 	./$(TESTING_OUTPUT)
