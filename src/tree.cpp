#include <cstdlib>
#include <cstdio>
#include <queue>
#include "tree.hpp"

/* Print the MOTHERFUCKING subtree beginning at this.
   Generally, just call this on the root and it'll print
   yo tree. */
void QTnode::Print()
{
  int i;
  std::queue <QTnode *> nodes;
  QTnode *t;

  nodes.push(this);

  while (!nodes.empty()) {
    t = nodes.front();
    nodes.pop();

    printf("Node %p\n", t);
    printf("Has x-bounds %lf -> %lf\n", t->x.beg, t->x.lim);
    printf("Has y-bounds %lf -> %lf\n", t->y.beg, t->y.lim);
    printf("Has %d particles.\n", t->particles.size());
    printf("Has %d children.\n", t->children.size());
    printf("Has parent %p\n", t->parent);

    for (i = 0; i < t->children.size(); i++) {
      printf("Has child %p\n", t->children[i]);
    }

    for (i = 0; i < t->children.size(); i++) {
      nodes.push(t->children[i]);
    }
  }
}

bool QTnode::insert(Particle p)
{
  int i;

  if (in_bounds(p)) {
    /* We are at a leaf node. Just insert. */
    if (children.size() == 0) {
      particles.push_back(p);
      return true;
    } else {
      for (i = 0; i < children.size(); i++) {
        if (children[i]->in_bounds(p)) {
          return children[i]->insert(p);
        }
      }
      /* Could not insert where it should fit. */
      return false;
    }
  } else {
    if (parent) {
      return parent->insert(p);
    } else {
      return false;
    }
  }
}

bool QTnode::in_bounds(Particle p)
{
  return (p.pos.x > x.beg && p.pos.x < x.lim) && (p.pos.y > y.beg && p.pos.y < y.lim);
}

QTnode::QTnode(QTnode *parent, double xbeg, double ybeg, double step)
{
  this->parent = parent;

  this->x.beg = xbeg;
  this->x.lim = xbeg+step;
  this->y.beg = ybeg;
  this->y.lim = ybeg+step;

  this->num_particles = 0;
}

QTnode *init_tree(int height, QTnode *parent)
{
  int i, j;
  double xbeg, ybeg, step;
  QTnode *t;

  if (height >= 0) {
    if (!parent) {
      t = new QTnode(parent, 0.0, 0.0, SIZE);
      init_tree(height-1, t);
    } else {
      step = (parent->x.lim - parent->x.beg) / 2.0;
      j = 0;
      for (ybeg = parent->y.beg; j < 2; ybeg += step) {
        i = 0;
        for (xbeg = parent->x.beg; i < 2; xbeg += step) {
          t = new QTnode(parent, xbeg, ybeg, step);
          parent->children.push_back(t);
          init_tree(height-1, t);
          i++;
        }
        j++;
      }
    }
  }

  return t;
}

void destroy_tree(QTnode *parent)
{
  int i;
  QTnode *t;
  std::vector <QTnode *> children;

  for (i = 0; i < parent->children.size(); i++) {
    children.push_back(parent->children[i]);
  }

  free(parent);

  for (i = 0; i < children.size(); i++) {
    destroy_tree(children[i]);
  }
}
