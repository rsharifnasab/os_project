
/*
 * clui library, by nima heydari nasab
 * @radio_nima
 * and some changes by roozbeh sharifnasab
 * @rsharifnasab
 */


#ifndef _CLUI
#define _CLUI

/*
 * common includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>


/*
 * includes for windows
 */
#if defined(_WIN32) || defined(_WIN64) || defined(WI32)

#define OS_UNIX 0
#include <conio.h>
#include <Windows.h>

/*
 * includes for unix-like systems
 * MacOS or linux
 */
#else

#define OS_UNIX 1
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#endif


/*
 * define colors that can be chosen
 * in change_color function
 */
#define COLOR_DEFAULT      0
#define COLOR_RED          1
#define COLOR_BOLD_RED     2
#define COLOR_GREEN        3
#define COLOR_BOLD_GREEN   4
#define COLOR_YELLOW       5
#define COLOR_BOLD_YELLOW  6
#define COLOR_BLUE         7
#define COLOR_BOLD_BLUE    8
#define COLOR_MAGENTA      9
#define COLOR_BOLD_MAGENTA 10
#define COLOR_CYAN         11
#define COLOR_BOLD_CYAN    12

/*
 * colors added in version2
 * this is use 8 bit terminal
 */
#define COLOR_ORANGE_1    13
#define COLOR_ORANGE_2    14
#define COLOR_ORANGE_3    15




void enable_raw_mode(){
#if OS_UNIX
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
#else

#endif
}

void disable_raw_mode(){
#if OS_UNIX
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
#else

#endif
}



bool is_keyboard_hit(){
#if OS_UNIX
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
#else
    return kbhit();
#endif
}




void clear_screen(){
#if OS_UNIX
    system("clear");
#else
    system("CLS");
#endif
}



/*
 * change output color of terminal
 * by RGB input
 * this will only work for 24-bit terminals
 * tested in linux terminals
 * and new Windows10 cmd and powershell
 */
void change_color_rgb(int r, int g, int b){
    printf("\033[38;2;%d;%d;%dm",r,g,b);
}

/*
 * change color in 8 bit terminal
 */
void change_color(int color) {
    printf("\033");

    const char *colors[] = {
        "[0m",
        "[0;31m",
        "[1;31m",
        "[0;32m",
        "[1;32m",
        "[0;33m",
        "[1;33m",
        "[0;34m",
        "[1;34m",
        "[0;35m",
        "[1;35m",
        "[0;36m",
        "[1;36m",

        "[38;5;202m",
        "[38;5;208m",
        "[38;5;214m"
    };
    if (color >= 0 && color <= 15) {
        printf("%s", colors[color]);
    }
}


void reset_color(){
    change_color(COLOR_DEFAULT);
}


void flush(){
    fflush(stdout);
    fflush(stderr);
}


void quit() {
    reset_color();
    disable_raw_mode();
    clear_screen();
    exit(0);
}


void sigint_handler(int dummy) {
    quit();
}

void init_clui(){
    clear_screen();
    signal(SIGINT, sigint_handler);
    enable_raw_mode();
}


#if OS_UNIX
int getch(){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}
#endif


void delay(int milli_seconds){
#if OS_UNIX
    usleep(1000*milli_seconds);
#else
    Sleep(milli_seconds);
#endif
}




int get_window_rows(){
#if OS_UNIX
    struct winsize max;
    ioctl(0, TIOCGWINSZ , &max);
    return max.ws_row;
#else
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif
}


int get_window_cols(){
#if OS_UNIX
    struct winsize max;
    ioctl(0, TIOCGWINSZ , &max);
    return max.ws_col;
#else
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#endif
}



#endif
