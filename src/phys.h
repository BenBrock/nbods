typedef struct {
  double x, y;
} f2;

typedef struct {
  f2 pos;
  f2 vel;
} Particle;

typedef struct {
  Particle p;
  Particle *next;
} Plist;

