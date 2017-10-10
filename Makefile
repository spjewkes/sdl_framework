CPP=g++
CPPFLAGS=-std=c++11 -Wall -Wextra
EXE=run_example

OBJ_DIR=obj
SRC_DIR=src

_DEPS=sdl_framework.h example.h
DEPS=$(patsubst %,$(SRC_DIR)/%,$(_DEPS))

_OBJ=main.o
OBJ=$(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

OS := $(shell uname)

ifeq ($(OS),Darwin)
# Mac OS
	CPPFLAGS+=-framework SDL2
else
# Assume Linux for now
	CPPFLAGS+=-lSDL2
endif

default: debug

debug: CPPFLAGS += -g
debug: build

release: CPPFLAGS += -O2
release: build

build: setup_build $(EXE)
	@echo "Build finished"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $^ -o $@

setup_build:
	@mkdir -p $(OBJ_DIR)

.PHONY: clean

clean:
	@echo "Cleaning"
	@rm -f $(OBJ_DIR)/*.o *~ $(SRC_DIR)/*~
