#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;

    // Turn off echoing the user input.
    // This mode is also used in terminals to input passwords.
    raw.c_lflag &= ~(ECHO | ICANON);


    // The second argument specifies when to apply the changes.
    //
    // #define TCSAFLUSH 2 /* drain output, flush input */
    // Wait for any pending output to be written to the terminal and discard
    // any input that hasn't been read.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    char c;
    while(read(STDIN_FILENO, &c, 1) == 1 && c !=  'q') {
        if(iscntrl(c)) {
            printf("%d\n", c);
        } else {
            printf("%d ('%c')\n", c, c);
        }
    }
    return 0;
}
