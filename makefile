CXX = g++
CFLAGS = -Wall -Wextra -pedantic -ggdb --std=c++11
# CURSES = -lncurses # ascii
CURSES = -lncursesw	# wide char

TARGET = main

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -I/opt/homebrew/opt/ncurses/include
	LIBS = -L/opt/homebrew/opt/ncurses/lib $(CURSES)
else ifeq ($(UNAME_S), Linux)
	LIBS = $(CURSES)
endif

.PHONY: all
all: main

main: main.cpp
	$(CXX) $(CFLAGS) -o main.exe main.cpp $(LIBS)
clean:
	rm -rf *.o *.exe *.dSYM *.gch