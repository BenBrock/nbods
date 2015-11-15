#include "graphics/displayimage.hpp"
#include "tree.hpp"
#include <queue>

int main(int argc, char **argv)
{
  cairo_surface_t *surface;
  cairo_t *context;
  int x, y, i,
      width, height, depth,
      screen, pressed_key;
  double r, b, v;
  QTnode *tree;

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

  /* Create the quad tree */
  tree = init_tree(3, NULL);
  for(i = 0; i < 1000; ++i) {
    tree->insert(phys_gen_particle());
  }

  while(!((*xwin)->should_close)) {

    /* Fill particles */
    tree->calc_global_accel();
    tree->move_shit();
    
    /* Wait on the input (also sync up to disable flickering) */
    if(input_ready(xwin)) {
      pressed_key = get_key(xwin);
    }

    /* Clear the surface with black */
    cairo_set_source_rgb(context, 0.0, 0.0, 0.0);
    cairo_paint(context);

    /* Draw the particles */
    std::queue <QTnode *> nodes;
    QTnode *t;

    nodes.push(tree);

    while (!nodes.empty()) {
      t = nodes.front();
      nodes.pop();

      if (!t->children.empty()) {
        for (i = 0; i < t->children.size(); i++) {
          nodes.push(t->children[i]);
        }
      } else {
        for (std::list <Particle>::iterator p = t->particles.begin(); p != t->particles.end(); p++) {
          v = f2_norm((*p).vel);
          if(v >= 0.4) {
            r = 1.0; b = 0.0;
          } else if(v < 0.5) {
            b = 1.0; r = 0.0;
          }
          cairo_set_source_rgba(context, (double)r, 0.0, (double)b, 1.0);
          cairo_rectangle(context, (*p).pos.x,
                          (*p).pos.y, 1e-3, 1e-3);
          cairo_fill(context);
        }
      }
    }

    /* Flush the X window */
    flush_input(xwin);
    update_screen(xwin);

    /* Get the new particles */
    phys_step(1/60.0);
  }
  
  destroy_tree(tree);
  cairo_destroy(context);
  cairo_surface_destroy(surface);
  xwindow_del(xwin);

  return 0;
}
