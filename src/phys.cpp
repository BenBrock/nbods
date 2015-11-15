#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.hpp"
#include "phys.hpp"

/* gravitational constant */
#define G 1.0e-3
#define RMIN 1.0e-2

Particle *particles;
int N;
double E0;

double get_energy()
{
  double E = 0.0;
  for (int i = 0; i < N; i++) {
    // kinetic
    double Ei = 0.5 * f2_dot(particles[i].vel, particles[i].vel);

    // potential
    for (int j = 0; j < N; j++) {
      if (i == j)
        continue;
      f2 r = f2_minus(particles[i].pos, particles[j].pos);
      double r_norm = f2_norm(r);
      Ei -= G / r_norm;
    }
    E += Ei;
  }
  return E;
}

void phys_init(int n)
{
  N = n;
  particles = (Particle *)calloc(N, sizeof(Particle));
  for (int i = 0; i < N; i++) {
    particles[i].pos.x = (double) rand() / RAND_MAX;
    particles[i].pos.y = (double) rand() / RAND_MAX;
    particles[i].vel.x = (double) rand() / RAND_MAX - 0.5;
    particles[i].vel.y = (double) rand() / RAND_MAX - 0.5;
  }
  E0 = get_energy();
}

void phys_step(double dt)
{
  for (int i = 0; i < N; i++) {
    f2 acc = {0, 0};
    for (int j = 0; j < N; j++) {
      if (i == j)
        continue;
      f2 r = f2_minus(particles[i].pos, particles[j].pos);
      double r_norm = f2_norm(r);
      if (r_norm < RMIN)
        continue;
      acc = f2_add(acc, f2_mult(r, -pow(r_norm, -3)));
    }
    acc = f2_mult(acc, G);
    particles[i].vel = f2_add(particles[i].vel, f2_mult(acc, dt));
    particles[i].pos = f2_add(particles[i].pos, f2_mult(particles[i].vel, dt));

    /* Bounce on walls */
    if (particles[i].pos.x < 0.0) {
      particles[i].pos.x *= -1;
      particles[i].vel.x *= -1;
    }
    else if (particles[i].pos.x >= 1.0) {
      particles[i].pos.x = 2.0 - particles[i].pos.x;
      particles[i].vel.x *= -1;
    }
    if (particles[i].pos.y < 0.0) {
      particles[i].pos.y *= -1;
      particles[i].vel.y *= -1;
    }
    else if (particles[i].pos.y >= 1.0) {
      particles[i].pos.y = 2.0 - particles[i].pos.y;
      particles[i].vel.y *= -1;
    }
  }

  /* Renormalization */
  double E = get_energy();
  for (int i = 0; i < N; i++) {
    particles[i].vel = f2_mult(particles[i].vel, E / E0);
  }
}

double asdf(double r)
{
  return 1.0 / r;
}

void phys_print()
{
  for (int i = 0; i < N; i++) {
    printf("pos: %lg, %lg\tvel: %lg, %lg\n", particles[i].pos.x, particles[i].pos.y, particles[i].vel.x, particles[i].vel.y);
  }
}
