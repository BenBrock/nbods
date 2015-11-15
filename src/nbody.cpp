#include "timing/timing.hpp"
#include "graphics/displayimage.hpp"
#include "tree.hpp"
#include <sys/stat.h>
#include <queue>
#include <string.h>

int main(int argc, char **argv)
{
  cairo_surface_t *surface;
  cairo_t *context;
  int x, y, i,
      width, height, depth,
      screen, pressed_key,
      png;
  double r, b, v;
  QTnode *tree;
  bool rendering;
  struct XWin **xwin;

  /* Get the arguments */
  if(argc != 3) {
    printf("Usage: nbody rendering resultsdir\n");
    exit(1);
  }
  if(*argv[1] == '0') {
    rendering = false;
  } else {
    rendering = true;
  }
  char buf[strlen(argv[2]) + 10];

  /* Set window size */
  width = 1024;
  height = 1024;
  depth = 32;
  
  /* Create the drawing surface */
  if(rendering) {
    /* Create the X11 window */
    xwin = (struct XWin **)calloc(sizeof(struct XWin *), 1);
    xwindow_init(width, height, depth, xwin);
    surface = cairo_xlib_surface_create((*xwin)->dsp, (*xwin)->win, DefaultVisual((*xwin)->dsp, screen), width, height);
    cairo_xlib_surface_set_size(surface, width, height);
  } else {
     surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height); 
  }
  context = cairo_create(surface);
  cairo_scale(context, width, height);

  phys_init(1000);

  png = 0;
  while(1) {
    if(rendering) {
      if((*xwin)->should_close) {
        break;
      }
      /* Wait on the input (also sync up to disable flickering) */
      if(input_ready(xwin)) {
        pressed_key = get_key(xwin);
      }
    }

    /* Clear the surface with black */
    cairo_set_source_rgb(context, 0.0, 0.0, 0.0);
    cairo_paint(context);

    for (int i = 0; i < N; i++) {
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
      cairo_set_source_rgba(context, (double)r, 0.0, (double)b, 0.2);
      cairo_rectangle(context, particles[i].pos.x - 1e-3,
                      particles[i].pos.y - 1e-3, 4e-3, 4e-3);
      cairo_fill(context);
    }
   
    if(rendering) {
      /* Flush the X window */
      flush_input(xwin);
      update_screen(xwin);
    } else {
      mkdir(argv[2], S_IRWXU | S_IRWXG);
      sprintf(buf, "%s/%05d.png", argv[2], png++);
      printf("Making %s\n", buf);
      cairo_surface_write_to_png (surface, buf);
    }

    /* Get the new particles */
    phys_step(1/60.0);
  }
  
  destroy_tree(tree);
  cairo_destroy(context);
  cairo_surface_destroy(surface);
  if(rendering) {
    xwindow_del(xwin);
  }

  return 0;
}
