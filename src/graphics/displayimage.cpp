#include "displayimage.hpp"

void update_screen(struct XWin **xwin)
{
  XFlush((*xwin)->dsp);
}

void change_args(int id, void *args, struct XWin **xwin)
{
  (*((*xwin)->keys) + (id - 1))->args = args;
}

void flush_input(struct XWin **xwin)
{
  XFlush((*xwin)->dsp);
  while(XPending((*xwin)->dsp)) {
    XNextEvent((*xwin)->dsp, (*xwin)->evt);
  }
}

int input_ready(struct XWin **xwin)
{
  FD_ZERO(&((*xwin)->input_fd));
  FD_SET((*xwin)->connectnum, &((*xwin)->input_fd));
  (*xwin)->tv.tv_usec = 16000;
  (*xwin)->tv.tv_sec = 0;
  if(select((*xwin)->connectnum + 1, &((*xwin)->input_fd), 0, 0, &((*xwin)->tv))) {
    return 1;
  } else {
    return 0;
  }
}

void handle_key(int id, struct XWin **xwin)
{
  (*(((*xwin)->keys) + (id - 1)))->function((*(((*xwin)->keys) + (id - 1)))->args);
}

int get_key(struct XWin **xwin)
{
  int i;
  switch((*xwin)->evt->type) {
    // Grab a key and return
    case(KeyPress):
      for(i = 0; i < (*xwin)->numkeys; ++i) {
        if((*xwin)->evt->xkey.keycode == (*(((*xwin)->keys) + i))->keycode) {
          return i + 1;
        }
      }
      break;
    // Handle window resizes
    case(ConfigureNotify):
      if((*xwin)->evt->xconfigure.width != (*xwin)->xres || 
         (*xwin)->evt->xconfigure.height != (*xwin)->yres) {
        (*xwin)->xres = (*xwin)->evt->xconfigure.width;
        (*xwin)->yres = (*xwin)->evt->xconfigure.height;
      }
      break;
    // Handle closing the window
    case(ClientMessage):
      if((*xwin)->evt->xclient.data.l[0] == (*xwin)->wmdelete) {
        (*xwin)->should_close = 1;
      }
      break;
    default:
      break;
  }
  return 0;
}

int create_key(char *keyvalue, void *(*function)(void *), 
                void *args, struct XWin **xwin)
{
  struct Key **key = (*xwin)->keys + (*xwin)->numkeys;
  *key = (struct Key *)calloc(sizeof(struct Key), 1);
  (*key)->keycode = XKeysymToKeycode((*xwin)->dsp, XStringToKeysym(keyvalue));
  (*key)->function = function;
  (*key)->args = args;
  return ++((*xwin)->numkeys);
}

void xwindow_init(int width, int height, int depth, struct XWin **xwin)
{
  unsigned int black;
  int screen;

  // Allocate some space for the xvars
  *xwin = (XWin *)calloc(sizeof(struct XWin), 1);
  (*xwin)->keys = (Key **)malloc(sizeof(struct Key *) * 20);
  (*xwin)->evt = (XEvent *)malloc(sizeof(XEvent));

  // Initialize the display
  (*xwin)->dsp = XOpenDisplay(NULL);
  if( !(*xwin)->dsp ){
    fprintf(stderr, "Couldn't get display. Abort.\n");
    exit(1);
  }
  
  // Get the screen
  screen = DefaultScreen((*xwin)->dsp);
  
  // Set the width and height of the window
  if(DisplayWidth((*xwin)->dsp, screen) >= width) {
    (*xwin)->xres = width;
  } else {
    (*xwin)->xres = DisplayWidth((*xwin)->dsp, screen);
  }
  if(DisplayHeight((*xwin)->dsp, screen) >= height) {
    (*xwin)->yres = height;
  } else {
    (*xwin)->yres = DisplayHeight((*xwin)->dsp, screen);
  }

  black = BlackPixel((*xwin)->dsp, screen);

  (*xwin)->win = XCreateSimpleWindow((*xwin)->dsp,
                            DefaultRootWindow((*xwin)->dsp),
                            0, 0,   // origin
                            (*xwin)->xres, (*xwin)->yres, // size
                            0, 0, // border width/clr
                            0);   // backgrd clr

  
  // Window Manager setup
  (*xwin)->wmdelete = XInternAtom((*xwin)->dsp, "WM_DELETE_WINDOW", True);
  XSetWMProtocols((*xwin)->dsp, (*xwin)->win, &((*xwin)->wmdelete), 1);

  (*xwin)->gc = XCreateGC((*xwin)->dsp, (*xwin)->win, 0, NULL);

  long eventMask = StructureNotifyMask;
  eventMask |= ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask;
  XSelectInput((*xwin)->dsp, (*xwin)->win, eventMask);

  XMapWindow((*xwin)->dsp, (*xwin)->win);

  // wait until window appears
  do { XNextEvent((*xwin)->dsp, (*xwin)->evt); } while ((*xwin)->evt->type != MapNotify);
  
  (*xwin)->width = width;
  (*xwin)->height = height;
  (*xwin)->depth = depth;
  (*xwin)->numkeys = 0;

  // Initialize variables used for FPS
  (*xwin)->connectnum = ConnectionNumber((*xwin)->dsp);

  // Shouldn't close immediately
  (*xwin)->should_close = 0;
}

void xwindow_del(struct XWin **xwin)
{
  XFreeGC((*xwin)->dsp, (*xwin)->gc);

  XDestroyWindow((*xwin)->dsp, (*xwin)->win);
  XCloseDisplay((*xwin)->dsp);

  free((*xwin)->keys);
  free((*xwin)->evt);
  free((*xwin)->ximage);
  free(*xwin);
}
