#
# Project config
#
PROJECT_NAME = spaceship-raylib
CC           = x86_64-w64-mingw32-gcc

# 
# Platform. One of:
# - PLATFORM_DESKTOP
# - PLATFORM_RPI
# - PLATFORM_ANDROID
# - PLATFORM_WEB
#
PLATFORM ?= PLATFORM_DESKTOP

# Define compiler flags:
#  -Wall                this enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros
#  -Wextra              this enables some extra warning flags that are not enabled by -Wall
#  -Werror              make all warnings into errors 
#
#  -Wfloat-equal        warn if floating-point values are used in equality comparisons (Deactivated due to raymath.h)
#  -Wuninitialized      warn if an object with automatic or allocated storage duration is used without having been initialized
#  -Winit-self          warn about uninitialized variables that are initialized with themselves
#
#  -Wno-missing-braces         deactivates the warning for missing braces
#  -Wno-unknown-pragmas        deactivates the warning for unknown pragmas
#  -Wno-error=unknown-pragmas  deactivates the error for unknown pragmas (allows the use of `#pragma region` and `#pragma endregion` outside Visual Studio C/C++ compiler)
#
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -std=c11             defines C language mode (standard C from 2011 revision)
#  -std=c++14           defines C++ language mode (standard C++ from 2014 revision)
#  -std=c17             defines C language mode (standard C from 2017 revision)
#
#  -D_DEFAULT_SOURCE    defined the _DEFAULT_SOURCE macro. Use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
FLAGS = -Wall -Werror -Wextra -Wuninitialized -Winit-self -Wno-missing-braces -Wno-unknown-pragmas -Wno-error=unknown-pragmas -std=c17 -D_DEFAULT_SOURCE  -D$(PLATFORM)

#
# Directories
#
RAYLIB_DIR  = #included with w64devkit
SRC_DIR     = src
SRC_SUBDIRS = $(shell find $(SRC_DIR) -type d)
INC_DIRS    = $(SRC_SUBDIRS)
LIB_DIRS    = libs $(RAYLIB_DIR)
BIN_DIR     = bin

#
# Files
#
SOURCES    = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJECTS    = $(SOURCES:$(SRC_DIR)/%.c=%.o)
EXECUTABLE = $(BIN_DIR)/$(PROJECT_NAME)

#
# Includes and libs (must be put AFTER source files)
#
INC_PATHS = $(addprefix -I ,$(INC_DIRS))
LIB_PATHS = $(addprefix -L ,$(LIB_DIRS))
LIBS      = -lraylib -lopengl32 -lgdi32 -lwinmm -lm

################
# Default rule #
################

.PHONY: help
help:
	@echo -e "\
	The available rules are:\n\
	- clean    Cleans the binaries folder\n\
	* debug    Debug rules\n\
	* testing  Testing rules\n\
	* release  Release rules\n\
	\n\
	\033[1m[*]\033[0m\n\
	These rules are available with the following sufixes:\n\
	- clean  Cleans the compiled binaries of the build\n\
	- prep   Cleans the related binaries\n\
	- build  prep + Compiles the code\n\
	- run    prep + build + Runs the compilated program\n\
	The format is \033[1m<rule>-<sufix>\033[0m\n"

###############
# Debug rules #
###############

DBG_NAME       = debug
DBG_DIR        = $(BIN_DIR)/$(DBG_NAME)
DBG_EXECUTABLE = $(EXECUTABLE)-$(DBG_NAME)
DBG_OBJECTS    = $(addprefix $(DBG_DIR)/, $(OBJECTS))
# Flags:
#  -00        no optimization
#  -g         include debug information on compilation
#  -DDEBUG    defines the DEBUG macro
#  -DTESTING  defines the TESTING macro
DBG_FLAGS = -O0 -g -DDEBUG -DTESTING

debug-clean:
	rm -rf $(DBG_DIR)

debug-prep:
	mkdir -p $(DBG_DIR) $(SRC_SUBDIRS:$(SRC_DIR)/%=$(DBG_DIR)/%)

debug-build: debug-prep $(DBG_EXECUTABLE)

debug-run: debug-build
	$(DBG_EXECUTABLE)

$(DBG_EXECUTABLE): $(DBG_OBJECTS)
	$(CC) $(FLAGS) $(DBG_FLAGS) -o $(DBG_EXECUTABLE) $^ $(INC_PATHS) $(LIB_PATHS) $(LIBS)

$(DBG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(DBG_FLAGS) -c -o $@ $< $(INC_PATHS) $(LIB_PATHS) $(LIBS)


#################
# Testing rules #
#################

TST_NAME       = testing
TST_DIR        = $(BIN_DIR)/$(TST_NAME)
TST_EXECUTABLE = $(EXECUTABLE)-$(TST_NAME)
TST_OBJECTS    = $(addprefix $(TST_DIR)/, $(OBJECTS))
# Flags:
#  -00        no optimization
#  -DTESTING  defines the TESTING macro
TST_FLAGS = -O0 -DTESTING

testing-clean:
	rm -rf $(TST_DIR)

testing-prep:
	mkdir -p $(TST_DIR) $(SRC_SUBDIRS:$(SRC_DIR)/%=$(TST_DIR)/%)

testing-build: testing-prep $(TST_EXECUTABLE)

testing-run: testing-build
	$(TST_EXECUTABLE)

$(TST_EXECUTABLE): $(TST_OBJECTS)
	$(CC) $(FLAGS) $(TST_FLAGS) -o $(TST_EXECUTABLE) $^ $(INC_PATHS) $(LIB_PATHS) $(LIBS)

$(TST_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(TST_FLAGS) -c -o $@ $< $(INC_PATHS) $(LIB_PATHS) $(LIBS)

#################
# Release rules #
#################

REL_NAME       = release
REL_DIR        = $(BIN_DIR)/$(REL_NAME)
REL_EXECUTABLE = $(EXECUTABLE)-$(REL_NAME)
REL_OBJECTS    = $(addprefix $(REL_DIR)/, $(OBJECTS))
# Flags:
#  -03                     maximum optimization
#  -s                      strip unnecessary data from build -> do not use in debug builds
#  -Wl,-subsystem,windows  no console on windows (no output log)
REL_FLAGS = -O3 -s -Wl,-subsystem,windows

release-clean:
	rm -rf $(REL_DIR)

release-prep:
	mkdir -p $(REL_DIR) $(SRC_SUBDIRS:$(SRC_DIR)/%=$(REL_DIR)/%)

release-build: release-prep $(REL_EXECUTABLE)

release-run: release-build
	$(REL_EXECUTABLE)

$(REL_EXECUTABLE): $(REL_OBJECTS)
	$(CC) $(FLAGS) $(REL_FLAGS) -o $(REL_EXECUTABLE) $^ $(INC_PATHS) $(LIB_PATHS) $(LIBS)

$(REL_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(REL_FLAGS) -c -o $@ $< $(INC_PATHS) $(LIB_PATHS) $(LIBS)

###############
# Other rules #
###############

clean:
	rm -rf $(BIN_DIR)
