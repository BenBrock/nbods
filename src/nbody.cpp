#include "phys.hpp"
#include "graphics/displayimage.hpp"

int main(int argc, char **argv)
{
  cairo_surface_t *surface;
  cairo_t *context;
  int x, y, i,
      width, height, depth,
      screen, pressed_key;
  double r, b, v;

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

  /* Initialize the physics web-scale cloud */
  phys_init(2000);

  while(!((*xwin)->should_close)) {
    
    /* Wait on the input (also sync up to disable flickering) */
    if(input_ready(xwin)) {
      pressed_key = get_key(xwin);
    }

    /* Clear the surface with black */
    cairo_set_source_rgb(context, 0.0, 0.0, 0.0);
    cairo_paint(context);

    /* Draw the particles */
    for(i = 0; i < N; ++i) {
      v = f2_norm(particles[i].vel);
      if(v >= 0.4) {
        r = 1.0; b = 0.0;
      } else if(v < 0.5) {
        b = 1.0; r = 0.0;
      }
      cairo_set_source_rgba(context, (double)r, 0.0, (double)b, 1.0);
      cairo_rectangle(context, particles[i].pos.x,
                      particles[i].pos.y, 2e-3, 2e-3);
      cairo_fill(context);
    }

    /* Flush the X window */
    flush_input(xwin);
    update_screen(xwin);

    /* Get the new particles */
    phys_step(1/60.0);
  }

  cairo_destroy(context);
  cairo_surface_destroy(surface);
  xwindow_del(xwin);

  return 0;
}
