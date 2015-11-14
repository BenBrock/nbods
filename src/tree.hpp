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

  QTnode(QTnode *parent, double xbeg, double ybeg, double step);
  void Print();
  bool in_bounds(Particle p);
  bool insert(Particle p);

  QTnode *parent;
  Bounds x, y;
  int num_particles;

  std::vector <QTnode *> children;
  std::list <Particle> particles;
};

QTnode *init_tree(int height, QTnode *parent);
void destroy_tree(QTnode *parent);
