# PHONY targets
.PHONY: clean run build build-linux build-windows

########## OS DETECTION ##########

ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
    EXE_EXT := .exe
    RM_DIR  := rmdir /S /Q
    MKDIR_P = if not exist "$(1)" mkdir "$(1)"
	LDFLAGS := -Llib/mingw64/ -lprovided -lSDL2 -lm
else
    OS_NAME := $(shell uname -s)
    EXE_EXT :=
    RM_DIR  := rm -rf
    MKDIR_P = mkdir -p "$(1)"
	LDFLAGS := -Llib/x86_64/ -lprovided -lSDL2 -lm
endif

########## VARIABLES ##########

SRC_DIR := src
OBJ_DIR := bin/obj
BIN_DIR := bin

CFLAGS  := -Wall -DDEBUG -I include/

EXECUTABLE := $(BIN_DIR)/executable$(EXE_EXT)

# recursive wildcard function
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# all .c sources (recursive)
SOURCES := $(call rwildcard,$(SRC_DIR)/,*.c)

# all .o objects
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

########## DEFAULT BUILD ##########

build: build-$(OS_NAME)

########## OS-SPECIFIC BUILDS ##########

build-Linux: $(EXECUTABLE)
build-Windows: $(EXECUTABLE)

########## LINK ##########

$(EXECUTABLE): $(OBJECTS)
	$(call MKDIR_P,$(BIN_DIR))
	gcc $(OBJECTS) $(LDFLAGS) -o $@

########## COMPILE ##########

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(call MKDIR_P,$(dir $@))
	gcc $(CFLAGS) -c $< -o $@

########## CLEAN / RUN ##########

clean:
	$(RM_DIR) "$(OBJ_DIR)"

run: build
	$(EXECUTABLE)
