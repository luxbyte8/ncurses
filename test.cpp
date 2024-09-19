#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include <ncursesw/ncurses.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::string;
using std::vector;

#define MAX_COLOR_PAIR 10

// global
WINDOW *new_win = nullptr;

// curses stuff
bool init_ncurses();
bool set_colors();
void set_color_pairs();
void set_custom_colors();
void cleanup_ncurses();

// matrix stuff
void start_matrix();
void hello_world();

int main()
{
    srand(time(nullptr));
    if(!init_ncurses()) return 1;
    // hello_world();
    start_matrix();
    cleanup_ncurses();
    return EXIT_SUCCESS;
} // main

bool init_ncurses()
{
    initscr();              // init ncurses
    /*
    new_win = newwin(0,0,0,0);
    if (!new_win) {
        cerr << "[-] ERR: failed to get new window" << endl;
        return false;
    }
    */
    noecho();               // no user input echo-ing
    cbreak();               // no user input buffering; no wait for 'enter'
    curs_set(1);            // 0 (hide), 1 (normal), 2 (intense)
    // scrollok(stdscr, TRUE); // will auto scroll when printing past last line
    keypad(stdscr, TRUE);   // keystrokes on this screen
    set_colors();           // set pairs and/or colors
    return true;
} // init_ncurses

bool set_colors()
{
    start_color();
    if(!has_colors()) {
        cerr << "[-] ERR: no colors" << endl;
        return false;
    }
    if(!can_change_color()) {
        cerr << "[-] ERR: cannot change color" << endl;
        return false;
    } 
    set_color_pairs();
    set_custom_colors();
    return true;
}

void set_color_pairs()
{
    // leave special pair 0; denotes no color; white on black
    for(int i = 1; i <= MAX_COLOR_PAIR; ++i) {
        // pair i: fg: i, bg: COLOR_BLACK
        init_pair(i, i, COLOR_BLACK);
    } 
}

void set_custom_colors()
{
    // leave special pair 0; denotes no color; white on black
    for(int i = 1; i <= MAX_COLOR_PAIR; ++i) {
        // pair i: fg: i, bg: COLOR_BLACK
        init_color(i, 0, i * (1000 / MAX_COLOR_PAIR), 0);
    }
    // make the last one custom, cause why not
    init_color(10, 1000, 1000, 1000);
}

void cleanup_ncurses()
{
    endwin();       // should auto. restore original state
}

void start_matrix()
{
    srand(time(nullptr));
    int height, width;
    while(true) {
        getmaxyx(stdscr, height, width);
        char c1 = 'A' + rand() % 26;
        char c2 = 'A' + rand() % 26;
        clear();
        for(int i = 0; i < height; ++i) {
            if(i != height/2) {
                color_set(8, nullptr);
                mvaddch(i, width / 2, c1);
            }
        }
        for(int i = 0; i < width; ++i) {
            if(i != width/2) {
                color_set(10, nullptr);
                mvaddch(height / 2, i, c2);
            }
        }
        refresh();
        napms(500);
    }
}

void hello_world()
{
    // A_BOLD causes different color/highlight
    // A_REVERSE reverses fg and bg; at least that's what I think
    int attrs = A_UNDERLINE | A_ITALIC | A_BLINK | A_REVERSE;
    for(int i = 0; i <= MAX_COLOR_PAIR; ++i) {
        color_set(i, nullptr);
        if(i == MAX_COLOR_PAIR/2) {
            attron(attrs);
            printw("[%d] Hello, World!\n", i);
            attroff(attrs);
        }
        else {
            printw("[%d] Hello, World!\n", i);
        }
    }
    refresh();
    getch();
}
// EOF