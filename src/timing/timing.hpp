#ifndef TIMING_H
#define TIMING_H

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define timing(a) gettimeofday(&beforetime, NULL); a; gettimeofday(&aftertime, NULL); timedifference(&aftertime, &beforetime);

extern struct timeval beforetime;
extern struct timeval aftertime;

void timedifference(struct timeval *x, struct timeval *y);

#endif
