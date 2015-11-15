#include "vec.hpp"

typedef struct {
  f2 pos;
  f2 vel;
  f2 accel;
} Particle;

extern Particle *particles;
extern int N;

void phys_init(int n);
void phys_step(double dt);
void phys_print();
f2 phys_get_force(f2 r);
Particle phys_move_particle(Particle particle);
Particle phys_gen_particle();
