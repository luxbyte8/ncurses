#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <ncursesw/ncurses.h>

using namespace std;

// basic ncurses initialization
void init();
void save_color_pairs(vector<vector<short>>&, int);
void save_orig_colors(vector<vector<short>>&, int);
void set_color_pairs();
void set_custom_colors();
void restore_color_pairs(const vector<vector<short>> &cp);
void restore_orig_colors(const vector<vector<short>> &cs);

// basic functionality testing
void hello_world();
void print_color_pairs();
void print_custom_colors();

int main()
{
    vector<vector<short>> orig_pairs;
    vector<vector<short>> orig_colors;
    
    init();
    
    hello_world();
    
    // use new color pairs and save them
    print_color_pairs();
    save_color_pairs(orig_pairs, 11);
    
    // save orig colors & use new custom colors
    save_orig_colors(orig_colors, 11);
    print_custom_colors();
    
    // without restoring colors this will
    // now use the custom colors
    print_color_pairs();

    // restore colors
    restore_orig_colors(orig_colors);
    print_color_pairs();

    endwin();
    return EXIT_SUCCESS;
} // main

// initializations ========================================
void init()
{
    initscr();
    noecho();
    cbreak();
    // keypad(WINDOW *, bool)   // must be called for every window
                                // to ge special keys to work on that window
    start_color();
    // checking for color will happen in other functions
}

void save_color_pairs(vector<vector<short>> &original, int count)
{
    for(int i = 0; i < count; ++i) {
        short fg, bg;
        pair_content(i, &fg, &bg);
        original.push_back({fg, bg});
    }
}

void save_orig_colors(vector<vector<short>> &original, int count)
{
    for(int i = 0; i < count; ++i) {
        short r, g, b;
        color_content(i, &r, &g, &b);
        original.push_back({r, g, b});
    }
}

void set_color_pairs()
{
    if(has_colors()) {
        // start_colors should create some default one
        // but let's override them and create our own
        // leave special pair 0; denotes no color; terminal default
        // you could modify it if you want but we'll leave it as is
        // Note: you must save pairs by using pair_content if you
        // want to restore to defaults at some point
        for(int i = 1; i <= 10; ++i) {
            // pair i -> (fg: i, bg: COLOR_BLACK)
            init_pair(i , i, COLOR_BLACK);
        }
    }
}

void set_custom_colors() 
{
    if(can_change_color()) {
        // if we can change colors, we can customize what color i
        // is based on rgb values. Note: you must save the previous
        // color by using color_content() function if you want to
        // restore at some point
        for(int i = 1; i <= 9; ++i) {
            // rgb values are from 0 to 1000
            init_color(i, 0, i * 100, 0);
        }
        // we'll make color 10 custom; in this case white
        init_color(10, 1000, 1000, 1000);
    }
}

void restore_color_pairs(const vector<vector<short>> &cp)
{
    for(size_t i = 0; i < cp.size(); ++i) {
        short fg = cp[i][0];
        short bg = cp[i][1];
        init_pair(i, fg, bg);
    }
}

void restore_orig_colors(const vector<vector<short>> &cs)
{
    for(size_t i = 0; i < cs.size(); ++i) {
        short r = cs[i][0];
        short g = cs[i][1];
        short b = cs[i][2];
        init_color(i, r, g, b);
    }
}

// basic functionality testing ============================
void hello_world()
{
    WINDOW *hello_world = newwin(0, 0, 0, 0);
    if(!hello_world) {
        cerr << "[-] ERR: unable to create new window" << endl;
        exit(1);
    }
    keypad(hello_world, TRUE);
    
    char name[256];
    
    // print some stuff
    mvwaddstr(hello_world, 0, 0, "Hello World\n");
    mvwaddstr(hello_world, 1, 0, "Enter your name: ");
    wrefresh(hello_world);
    echo();     // so user can see what they type
    wgetstr(hello_world, name);
    noecho();   // disable
    wprintw(hello_world, "Hello, %s\n", name);
    wprintw(hello_world, "Hit 'enter' to continue");
    wrefresh(hello_world);
    wgetch(hello_world);

    // clean up
    delwin(hello_world);
    hello_world = nullptr;
}

void print_color_pairs()
{
    WINDOW *color_win = newwin(0, 0, 0, 0);
    if(!color_win) {
        cerr << "[-] ERR: unable to create new window" << endl;
        exit(1);
    }
    keypad(color_win, TRUE);

    if(!has_colors()) {
        cerr << "[-] ERR: has no colors" << endl;
        exit(1);
    }
    set_color_pairs();

    // depending on terminal settings, A_BOLD
    // may print differently
    wattron(color_win, A_BOLD);
    mvwaddstr(color_win, 0, 0, "Using custom color pairs\n");
    wrefresh(color_win);
    wattroff(color_win, A_BOLD);
    for(int i = 0; i <= 10; ++i) {
        wcolor_set(color_win, i, nullptr);
        wprintw(color_win, "Using color pair: %d\n", i);
        wrefresh(color_win);
    }
    wprintw(color_win, "Hit 'enter' to continue");
    wrefresh(color_win);
    wgetch(color_win);

    delwin(color_win);
    color_win = nullptr;
}

void print_custom_colors()
{
    WINDOW *color_win = newwin(0, 0, 0, 0);
    if(!color_win) {
        cerr << "[-] ERR: unable to create new window" << endl;
        exit(1);
    }
    keypad(color_win, TRUE);

    if(has_colors()) {
        set_color_pairs();
    }
    if(can_change_color()) {
        set_custom_colors();
    }
    else {
        cerr << "[-] ERR: cannot change colors" << endl;
        exit(1);
    }

    wprintw(color_win, "Using custom colors\n");
    wrefresh(color_win);
    for(int i = 0; i <= 10; ++i) {
        wcolor_set(color_win, i, nullptr);
        wprintw(color_win, "Using color %d\n", i);
        wrefresh(color_win);
    }
    wprintw(color_win, "Hit 'enter' to continue");
    wrefresh(color_win);
    wgetch(color_win);
    
    delwin(color_win);
    color_win = nullptr;
}

// EOF