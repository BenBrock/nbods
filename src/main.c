#include "phys.h"
#include "graphics/displayimage.h"

#define N 10000

Particle particles[N];

int main(int argc, char **argv)
{
  cairo_surface_t *surface;
  cairo_t *context;
  int x, y, i,
      width, height, depth,
      screen;
  Drawable da;

  /* Set window size */
  width = 1024;
  height = 1024;
  depth = 32;

  /* Create the X11 window */
  struct XWin **xwin = calloc(sizeof(struct XWin *), 1);
  xwindow_init(width, height, depth, xwin);
  
  /* Create the drawing surface */
  surface = cairo_xlib_surface_create((*xwin)->dsp, da, DefaultVisual((*xwin)->dsp, screen), width, height);
  cairo_xlib_surface_set_size(surface, x, y);
  context = cairo_create(surface);
  cairo_scale(context, width, height);

  /* Fill the array of particles */
  srand((unsigned int)time(NULL));
  for(i = 0; i < N; ++i) {
    particles[i].pos.x = (double)rand() / (double)RAND_MAX;
    particles[i].pos.y = (double)rand() / (double)RAND_MAX;
  }
  
  /* Draw the background */
  cairo_rectangle(context, 0, 0, 1, 1);
  cairo_fill(context);

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

  cairo_destroy(context);
  cairo_surface_destroy(surface);

  return 0;
}
