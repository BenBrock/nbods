#include "timing.hpp"

struct timeval beforetime;
struct timeval aftertime;

void timedifference(struct timeval *x, struct timeval *y)
{
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  printf("%ld.%06ld\n", x->tv_sec - y->tv_sec, x->tv_usec - y->tv_usec);
}
