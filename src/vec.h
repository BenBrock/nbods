#pragma once
#include <math.h>

typedef struct {
	double x, y;
} f2;


inline f2 f2_add(f2 a, f2 b) {
	return (f2) {a.x + b.x, a.y + b.y};
}

inline f2 f2_minus(f2 a, f2 b) {
	return (f2) {a.x - b.x, a.y - b.y};
}

inline f2 f2_mult(f2 a, double s) {
	return (f2) {a.x * s, a.y * s};
}

inline f2 f2_div(f2 a, double s) {
	return (f2) {a.x / s, a.y / s};
}

inline double f2_norm(f2 a) {
	return hypot(a.x, a.y);
}

/* Assumes a is not zero */
inline f2 f2_normalize(f2 a) {
	return f2_div(a, f2_norm(a));
}

inline double f2_dot(f2 a, f2 b) {
	return a.x*b.x + a.y*b.y;
}
