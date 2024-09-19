CXX = g++
CFLAGS = -Wall -Wextra -pedantic -ggdb --std=c++11
# CURSES = -lncurses # ascii
CURSES = -lncursesw	# wide char

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -I/opt/homebrew/opt/ncurses/include
	LIBS = -L/opt/homebrew/opt/ncurses/lib $(CURSES)
else ifeq ($(UNAME_S), Linux)
	LIBS = $(CURSES)
endif

test: test.cpp
	$(CXX) $(CFLAGS) -o test.exe test.cpp $(LIBS)
clean:
	rm -rf *.o *.exe *.dSYM *.gch