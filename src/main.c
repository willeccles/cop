// make sure we have SIGWINCH
#ifdef __APPLE__
# define _DARWIN_C_SOURCE
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
# define __BSD_VISIBLE 1
#endif

#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "libcop.h"

struct termios oldterm;
static atomic_int rows, cols;

static int SetupTerm(void) {
  if (tcgetattr(STDIN_FILENO, &oldterm) < 0) {
    perror("tcgetattr");
    return -1;
  }

  setvbuf(stdout, NULL, _IOFBF, 0);

  struct termios newterm = oldterm;
  newterm.c_oflag &= ~OPOST;
  newterm.c_lflag &= ~(ECHO | ICANON);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &newterm) < 0) {
    perror("tcsetattr");
    return -1;
  }

  printf("\033[?1049h"  // alternate screen buffer
         "\033[?7l"     // disable line wrapping
         "\033[?25l"    // hide cursor
         "\033[2J"      // clear screen
         "\033[2;%dr",  // limit scrolling region
         rows - 1);

  return 0;
}

static void ResetTerm(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm) < 0) {
    perror("tcsetattr");
  }

  printf("\033[?7h"       // enable line wrapping
         "\033[?25h"      // unhide cursor
         "\033[r"         // reset scroll region
         "\033[?1049l");  // restore main screen

  fflush(stdout);
}

static int GetTermSize(void) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0) {
    perror("ioctl");
    return -1;
  }

  rows = ws.ws_row;
  cols = ws.ws_col;

  return 0;
}

static void SigHandler(int sig) {
}

int main(int argc, char** argv) {
  if (GetTermSize() < 0) {
    exit(1);
  }

  if (SetupTerm() < 0) {
    exit(1);
  }

  signal(SIGWINCH, SigHandler);
  signal(SIGWINCH, SigHandler);
  signal(SIGWINCH, SigHandler);
  signal(SIGWINCH, SigHandler);
  signal(SIGWINCH, SigHandler);
  signal(SIGWINCH, SigHandler);

  atexit(ResetTerm);

  SayHi();


  return 0;
}
