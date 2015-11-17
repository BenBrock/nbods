#include <vector>
#include <list>
#include "phys.hpp"

#define SIZE 1.0

typedef struct {
  double beg;
  double lim;
} Bounds;

class QTnode {
public:

  QTnode(QTnode *parent, double xbeg, double ybeg, double step, int height);
  void Print();

  bool in_bounds(Particle p);
  bool insert(Particle p);

  int get_num_particles();
  void recalc_num_particles();

  void calc_global_accel();
  void move_shit();
  f2 calc_accel(Particle p);

  QTnode *parent;
  Bounds x, y;
  int num_particles;
  int height;

  f2 get_centroid();

  std::vector <QTnode *> children;
  std::list <Particle> particles;
  int psize;
};

QTnode *init_tree(int height, QTnode *parent);
void destroy_tree(QTnode *parent);
