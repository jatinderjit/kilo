#include <termios.h>
#include <unistd.h>

void enableRawMode() {
    struct termios raw;

    // #define STDIN_FILENO 0 /* standard input file descriptor */
    tcgetattr(STDIN_FILENO, &raw);

    // Turn off echoing the user input.
    // This mode is also used in terminals to input passwords.
    raw.c_lflag &= ~(ECHO);


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
    while(read(STDIN_FILENO, &c, 1) == 1 && c !=  'q');
    return 0;
}
