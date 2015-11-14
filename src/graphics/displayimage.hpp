#ifndef DISPLAYIMAGE_H
#define DISPLAYIMAGE_H

#include <cairo.h>
#include <X11/Xlib.h>
#include <cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// Stores a key and what it should do
struct Key
{
  KeyCode keycode;
  void *(*function)(void *);
  void *args;
};

struct XWin
{
  // Resolution of the window
  int xres, yres;

  // Generic X11 stuff
  Atom wmdelete;
  XEvent *evt;
  Display *dsp;
  Drawable win;
  Visual *visual;
  GC gc;
  
  // The image and attributes
  XImage *ximage;
  int width, height, depth;

  // Vars for FPS
  int connectnum;
  fd_set input_fd;
  struct timeval tv;

  int should_close;

  // Key events
  int numkeys;
  struct Key **keys;
};

void update_screen(struct XWin **xwin);
int  input_ready  (struct XWin **xwin);
void change_args  (int it, void *args, struct XWin **xwin);
void flush_input  (struct XWin **xwin);
int  get_key      (struct XWin **xwin);
void handle_key   (int id, struct XWin **xwin);
int create_key   (char *keyvalue, void *(*function)(void *), void *args,
                   struct XWin **xwin);
void xwindow_init (int width, int height, int depth, struct XWin **xwin);
void xwindow_del  (struct XWin **xwin);

#endif
