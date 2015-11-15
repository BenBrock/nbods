#include <cstdlib>
#include <cstdio>
#include "tree.hpp"
#include <unistd.h>
#include <ctime>

Particle gen_particle();

int main(int argc, char **argv)
{
  time_t begin, end;
  QTnode *tree;

  begin = time(0);
  tree = init_tree(3, NULL);
  end = time(0);

  printf("%lu to init.\n", end - begin);

  begin = time(0);
  for (int i = 0; i < 1000; i++) {
    tree->insert(gen_particle());
  }
  end = time(0);

  printf("%lu to insert.\n", end - begin);

  // tree->Print();

  begin = time(0);
  destroy_tree(tree);
  end = time(0);

  printf("%lu to destroy.\n", end - begin);

  sleep(30);

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
