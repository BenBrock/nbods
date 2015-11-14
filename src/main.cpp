#include <cstdlib>
#include <cstdio>
#include "tree.hpp"

Particle gen_particle();

int main(int argc, char **argv)
{
  QTnode *tree;

  tree = init_tree(2, NULL);

  for (int i = 0; i < 100; i++) {
    tree->insert(gen_particle());
  }

  tree->Print();

  destroy_tree(tree);

  return 0;
}

Particle gen_particle()
{
  Particle p;

  p.pos = (f2) {drand48(), drand48()};
  p.vel = (f2) {0.0, 0.0};
  p.accel = (f2) {0.0, 0.0};

  return p;
}
