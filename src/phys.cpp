#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "vec.hpp"
#include "phys.hpp"

/* gravitational constant */
#define G 1.0e-3
#define DT 1.0e-4
#define RMIN 1.0e-2
// #define RENORMALIZATION_ENABLE

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
#if RENORMALIZATION_ENABLE
  E0 = get_energy();
#endif
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
#if RENORMALIZATION_ENABLE
  double E = get_energy();
  for (int i = 0; i < N; i++) {
    particles[i].vel = f2_mult(particles[i].vel, E / E0);
  }
#endif
}


f2 phys_get_force(f2 r)
{
  double r_norm = f2_norm(r);
  if (r_norm < RMIN)
    return (f2) {0.0, 0.0};

  return f2_mult(r, -G / (r_norm*r_norm*r_norm));
}


Particle phys_move_particle(Particle p)
{
  p.vel = f2_add(p.vel, f2_mult(p.accel, DT));
  p.pos = f2_add(p.pos, f2_mult(p.vel, DT));

  /* Bounce on walls */
  if (p.pos.x < 0.0) {
    p.pos.x *= -1;
    p.vel.x *= -1;
  }
  else if (p.pos.x >= 1.0) {
    p.pos.x = 2.0 - p.pos.x;
    p.vel.x *= -1;
  }
  if (p.pos.y < 0.0) {
    p.pos.y *= -1;
    p.vel.y *= -1;
  }
  else if (p.pos.y >= 1.0) {
    p.pos.y = 2.0 - p.pos.y;
    p.vel.y *= -1;
  }

  return p;
}

Particle phys_gen_particle()
{
  Particle p;

  p.pos = (f2) {drand48(), drand48()};
  p.vel = (f2) {drand48() - 0.5, drand48() - 0.5};
  p.accel = (f2) {0.0, 0.0};

  return p;
}

void phys_print()
{
  for (int i = 0; i < N; i++) {
    printf("pos: %lg, %lg\tvel: %lg, %lg\n", particles[i].pos.x, particles[i].pos.y, particles[i].vel.x, particles[i].vel.y);
  }
}
