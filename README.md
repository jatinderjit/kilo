# Kilo

We'll try to make [Antirez's kilo](http://antirez.com/news/108) by following
this [tutorial](https://viewsourcecode.org/snaptoken/kilo/).

We'll try to learn something extra on the way.

## 1. Setup

* Install C
* Create basic C file
* Create Makefile

## 2. Raw Mode

### 2.1 Read Input

Read 1 byte at a time from STDIN into a char until there are no more bytes.

By default, the terminal starts in **canonical mode**, also called **cooked
mode**. In this mode, keyboard input is sent to the program only when the user
presses enter. This is useful, as it allows the user to correct the input by
pressing backspace. But it is not useful for more complex user interfaces - like
text editors. We want to process each keypress as it comes up.

What we want is **raw mode**. Unfortunately, there's not a simple switch that we
can flip to enter raw mode. It is achieved by turning off a lot of flags. We'll
do that gradually in this chapter.

Press `ctrl+d` to "input" EOF. Press `ctrl+c` to signal the process to
terminate immediately.

### 2.2 Press `q` to quit

Quit reading whenever we read the letter `q`.

The program will read one line at a time. When it reaches `q`, it will stop
reading from STDIN. The unread buffer in the input queue might be fed into the
shell after the program exits.

### 2.3 Turn off echoing

Turn off the `ECHO` feature, which causes each key the user types to be printed
to the terminal, so that user can see what they typed. It is useful in canonical
mode, but gets in our way.

```c
// c_lflag: "Local" flags - dumping ground for other state
struct termios {
    tcflag_t        c_iflag;        /* input flags */
    tcflag_t        c_oflag;        /* output flags */
    tcflag_t        c_cflag;        /* control flags */
    tcflag_t        c_lflag;        /* local flags */
    cc_t            c_cc[NCCS];     /* control chars */
    speed_t         c_ispeed;       /* input speed */
    speed_t         c_ospeed;       /* output speed */
};

#define STDIN_FILENO 0 /* standard input file descriptor */

// Bitflag
#define ECHO 0x00000008 /* enable echoing */

// Wait for any pending output to be written to the terminal and discard any
// input that hasn't been read
#define TCSAFLUSH 2 /* drain output, flush input */
```

Read more about [terminal](./docs/terminal.md).
