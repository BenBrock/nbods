#include "phys.hpp"
#include "graphics/displayimage.h"

#define N 10000

Particle particles[N];

int main(int argc, char **argv)
{
  cairo_surface_t *surface;
  cairo_t *context;
  int x, y, i,
      width, height, depth,
      screen, pressed_key;

  /* Set window size */
  width = 1024;
  height = 1024;
  depth = 32;

  /* Create the X11 window */
  struct XWin **xwin = (struct XWin **)calloc(sizeof(struct XWin *), 1);
  xwindow_init(width, height, depth, xwin);
  
  /* Create the drawing surface */
  surface = cairo_xlib_surface_create((*xwin)->dsp, (*xwin)->win, DefaultVisual((*xwin)->dsp, screen), width, height);
  cairo_xlib_surface_set_size(surface, width, height);
  context = cairo_create(surface);
  cairo_scale(context, width, height);

  while(!((*xwin)->should_close)) {
    
    /* Wait on the input (also sync up to disable flickering) */
    if(input_ready(xwin)) {
      pressed_key = get_key(xwin);
    }

    /* Clear the surface with black */
    cairo_set_source_rgb(context, 0.0, 0.0, 0.0);
    cairo_paint(context);

    /* Fill the array of particles */
    printf("Filling array\n");
    fflush(stdout);
    srand((unsigned int)time(NULL));
    for(i = 0; i < N; ++i) {
      particles[i].pos.x = (double)rand() / (double)RAND_MAX;
      particles[i].pos.y = (double)rand() / (double)RAND_MAX;
    }
    printf("Finished filling array\n");
    fflush(stdout);

    /* Draw the particles */
    cairo_set_source_rgba(context, 1.0, 1.0, 1.0, 1.0);
    for(i = 0; i < N / 3; ++i) {
      cairo_rectangle(context, particles[i].pos.x,
                      particles[i].pos.y, 2e-3, 2e-3);
    }
    cairo_fill(context);
    cairo_set_source_rgba(context, 1.0, 1.0, 1.0, 1.0);
    for(i = (N / 3) + 1; i < ((2 * N) / 3); ++i) {
      cairo_rectangle(context, particles[i].pos.x,
                      particles[i].pos.y, 2e-3, 2e-3);
    }
    cairo_fill(context);
    cairo_set_source_rgba(context, 1.0, 1.0, 1.0, 0.1);
    for(i = ((2 * N) / 3) + 1; i < N; ++i) {
      cairo_rectangle(context, particles[i].pos.x,
                      particles[i].pos.y, 2e-3, 2e-3);
    }
    cairo_fill(context);

    /* Flush the X window */
    flush_input(xwin);
    update_screen(xwin);
  }

  cairo_destroy(context);
  cairo_surface_destroy(surface);
  xwindow_del(xwin);

  return 0;
}
