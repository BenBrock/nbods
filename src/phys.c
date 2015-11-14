#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"

/* gravitational constant */
#define G 1.0


typedef struct {
    f2 pos;
    f2 vel;
} Particle;


Particle *particles;
int N;


void phys_init(int n) {
    N = n;
    particles = calloc(N, sizeof(Particle));
    for (int i = 0; i < N; i++) {
        particles[i].pos.x = (double) rand() / RAND_MAX;
        particles[i].pos.y = (double) rand() / RAND_MAX;
        particles[i].vel.x = (double) rand() / RAND_MAX - 0.5;
        particles[i].vel.y = (double) rand() / RAND_MAX - 0.5;
    }
}

void phys_step(double dt) {
    for (int i = 0; i < N; i++) {
        f2 acc = {0, 0};
        for (int j = 0; j < N; j++) {
        }
        acc = f2_mult(acc, G);
        particles[i].vel = f2_add(particles[i].vel, f2_mult(acc, dt));
        particles[i].pos = f2_add(particles[i].pos, f2_mult(particles[i].vel, dt));
    }
}

void phys_print() {
    for (int i = 0; i < N; i++) {
        printf("pos: %lg, %lg\tvel: %lg, %lg\n", particles[i].pos.x, particles[i].pos.y, particles[i].vel.x, particles[i].vel.y);
    }
}
