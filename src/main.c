#include "phys.h"
#include "graphics/displayimage.h"

#define N 10000
#define X 8192
#define Y 8192

Particle particles[N];

int main(int argc, char **argv)
{
  cairo_surface_t *surface;
  cairo_t *context;
  int x, y, i;
  
  /* Create the drawing surface */
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, X, Y);
  context = cairo_create(surface);
  cairo_scale(context, X, Y);

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
  cairo_surface_write_to_png(surface, "hello.png");
  cairo_surface_destroy(surface);

  return 0;
}
