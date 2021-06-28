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
struct termios {  /* termios.h */
    tcflag_t        c_iflag;        /* input flags */
    tcflag_t        c_oflag;        /* output flags */
    tcflag_t        c_cflag;        /* control flags */
    tcflag_t        c_lflag;        /* local flags */
    cc_t            c_cc[NCCS];     /* control chars */
    speed_t         c_ispeed;       /* input speed */
    speed_t         c_ospeed;       /* output speed */
};

#define STDIN_FILENO 0 /* unistd.h: standard input file descriptor */

// Bitflag
#define ECHO 0x00000008 /* termios.h: enable echoing */

// Wait for any pending output to be written to the terminal and discard any
// input that hasn't been read
#define TCSAFLUSH 2 /* termios.h: drain output, flush input */
```

Read more about [terminal](./docs/terminal.md).

### 2.4 Disable raw mode at exit

Restore the terminals original attributes when the program exits.

```c
// Register a function to be called on exit.
// The function will be called automatically when the program exits, whether by
// returning from `main` or by calling the `exit` function.
//
// At least 32 functions can always be registered, and more are allowed as long
// as sufficient memory can be allocated.
atexit(disableRawMode);  /* stdlib.h */
```

Now the unread buffer after the letter `q` won't be fed into the shell after the
program quits, because of the `TCSAFLUSH` option passed to `tcsetattr` when the
program exits. It discards any unread input before applying the changes to the
terminal.

### 2.5 Turn off canonical mode

```c
// Read the input in lines
#define ICANON 0x00000100 /* termios.h: canonicalize input lines */
```

Now the program will quit as soon as we press `q`.

### 2.6 Display keypresses

Print each byte as it's `read()`. Print only the ASCII value also if it's a
printable character.

```c
// ASCII codes 0-31 and 127 are control characters.
// ASCII codes 32-126 are printable characters.
iscntrl(c);  /* ctype.h: is CONTROL type character  */
```

We can see that several ordinary keys translate to characters: `Tab`, `Enter`,
`Page Down`, `Home`, `End`, `Backspace`, `Delete`, and a few key combinations
with `Ctrl` (`Ctrl-A`, `Ctrl-D`, etc).

* `Ctrl-A` - `Ctrl-Z` translate to ASCII code 1-26, although a few of them won't
  be visible (`Ctrl-C`, `Ctrl-Z`, `Ctrl-S`, `Ctrl-Q`).
* `Ctrl-S` tells the program to stop sending output. `Ctrl-Q` resumes it.
* A few keys translate to 3 or 4 bytes - starting with `27`, `[`, and then one
  or two other characters.
* `Backspace` is code 127. and `Delete` is a 4 byte sequence.
