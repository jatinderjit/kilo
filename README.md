# Kilo

We'll try to make [Antirez's kilo](http://antirez.com/news/108) by following
this [tutorial](https://viewsourcecode.org/snaptoken/kilo/).

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
