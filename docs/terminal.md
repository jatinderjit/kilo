# Terminal

(copied in an unstructured manner)

There are a large number of things taken for granted on a terminal: `^C` or `Z`
(to kill or stop the foreground process), or handling these as normal keystrokes
in some programs. These signal's go to the remote processes when you ssh.

There's an involvement of the unix terminal emulation layer and the termios
interface.

## tty

### Terminal Special Files

Each hardware terminal port on the system usually has two terminal special
device files associated with it in `/dev/` (example, `/dev/tty03`). When a user
logs into the system on one of these hardware terminal ports, the system has
already opened the associated device and prepared the line for normal
interactive use.

There is also a special case of a terminal file that connects not to a hardware
terminal port, but to another program on the other side. These special terminal
devices are called `pty`s (pseudo terminals) and provide the mechanism necessary
to give users the same interface to the system when logging in over a network.

When an interactive user logs in, the system prepares the line to behave in a
certain way (called a _line discipline_).

### Line disciplines

A terminal file is used like any other file in the system in that it can be
opened, read, and written to using standard system calls. For each existing
terminal file, there is a software processing module called a _line discipline_
associated with it. The _line discipline_ essentially glues the low level device
driver code with the high level generic interface routines (such as read(2)
and write(2)), and is responsible for implementing the semantics associated with
the device.

When a terminal file is first opened by a program, the default line discipline
called the termios line discipline is associated with the file. This is the
primary line discipline that is used in most cases and provides the semantics
that users normally associate with a terminal. When the termios line discipline
is in effect, the terminal file behaves and is operated according to the rules
described in termios.

## pty

The pty driver provides support for a device-pair termed a _pseudo terminal_. A
pseudo terminal is a pair of character devices, a master device and a slave
device.

The slave device provides to a process an interface identical to that
described in [tty(4)]. However, whereas all other devices which provide the
interface described in tty(4) have a hardware device of some sort behind them,
the slave device has, instead, another process manipulating it through the
master half of the pseudo terminal. That is, anything written on the master
device is given to the slave device as input and anything written on the slave
device is presented as input on the master device.

## Job Control in a Nutshell

Every process is associated with a particular process group and session. The
grouping is hierarchical: every member of a particular process group is a member
of the same session. This structuring is used in managing groups of related
processes for purposes of job control; that is, the ability from the keyboard
(or from program control) to simultaneously stop or restart a complex command (a
command composed of one or more related processes).

The grouping into process groups allows delivering of signals that stop or start
the group as a whole, along with arbitrating which process group has access to
the single controlling terminal. The grouping at a higher layer into sessions is
to restrict the job control related signals and system calls to within processes
resulting from a particular instance of a "login".

Typically, a session is created when a user logs in, and the login terminal is
set up to be the controlling terminal; all processes spawned from that login
shell are in the same session, and inherit the controlling terminal. A job
control shell operating interactively (that is, reading commands from a
terminal) normally groups related processes together by placing them into the
same process group. A set of processes in the same process group is collectively
referred to as a "job".

When the foreground process group of the terminal is the same as the process
group of a particular job, that job is said to be in the _foreground_. When the
process group of the terminal is different than the process group of a job (but
is still the controlling terminal), that job is said to be in the _background_.

Normally the shell reads a command and starts the job that implements that
command. If the command is to be started in the foreground (typical), it sets
the process group of the terminal to the process group of the started job, waits
for the job to complete, and then sets the process group of the terminal back to
its own process group (it puts itself into the foreground).

If the job is to be started in the background (as denoted by the shell operator
"&"), it never changes the process group of the terminal and doesn't wait for
the job to complete (that is, it immediately attempts to read the next command).

If the job is started in the foreground, the user may type a character (usually
`^Z`) which generates the terminal stop signal (SIGTSTP) and has the affect of
stopping the entire job. The shell will notice that the job stopped (see
[wait(2)]), and will resume running after placing
itself in the foreground.

The shell also has commands for placing stopped jobs in the background, and for
placing stopped or background jobs into the foreground.

### Orphaned Process Groups

An orphaned process group is a process group that has no process whose parent is
in a different process group, yet is in the same session. Conceptually it means
a process group that doesn't have a parent that could do anything if it were to
be stopped. For example, the initial login shell is typically in an orphaned
process group. Orphaned process groups are immune to keyboard generated stop
signals and job control signals resulting from reads or writes to the
controlling terminal.

### The Controlling Terminal

The Controlling Terminal may belong to a process as its controlling terminal.
Each process of a session that has a controlling terminal has the same
controlling terminal. A terminal may be the controlling terminal for at most one
session. The controlling terminal for a session is allocated by the session
leader by issuing the TIOCSCTTY ioctl. A controlling terminal is never acquired
by merely opening a terminal device file. When a controlling terminal becomes
associated with a session, its foreground process group is set to the process
group of the session leader.

The controlling terminal is inherited by a child process during a [fork(2)]
function call. A process relinquishes its controlling terminal when it creates a
new session with the [setsid(2)] function; other processes remaining in the old
session that had this terminal as their controlling terminal continue to have
it. A process does not relinquish its controlling terminal simply by closing all
of its file descriptors associated with the controlling terminal if other
processes continue to have it open.

When a controlling process terminates, the controlling terminal is disassociated
from the current session, allowing it to be acquired by a new session leader.
Subsequent access to the terminal by other processes in the earlier session will
be denied, with attempts to access the terminal treated as if modem disconnect
had been sensed.

## <termios.h>

Provides terminal interface supported on
[tty](https://www.howtogeek.com/428174/what-is-a-tty-on-linux-and-how-to-use-the-tty-command/
"Teletypewriter") asynchronous communication ports.

## References

* [termios(4) - NetBSD Manual Pages](https://man.netbsd.org/termios.4)
* [tty(4) - NetBSD Manual Pages](https://man.netbsd.org/tty.4)
* [General Terminal Interface](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap11.html)
* [A Brief Introduction to Termios](https://blog.nelhage.com/2009/12/a-brief-introduction-to-termios/)

[fork(2)]: https://man.netbsd.org/fork.2
[pty(4)]: https://man.netbsd.org/pty.4
[setsid(2)]: https://man.netbsd.org/setsid.2
[termios(4)]: https://man.netbsd.org/termios.4
[tty(4)]: https://man.netbsd.org/tty.4
[wait(2)]: https://man.netbsd.org/wait.2
