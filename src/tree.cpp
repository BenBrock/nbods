#include <cstdlib>
#include <cstdio>
#include <queue>
#include "tree.hpp"

#define THETA 0.5

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
    printf("Has %d particles.\n", t->psize);
    printf("Has %lu children.\n", t->children.size());
    printf("Has parent %p\n", t->parent);

    for (i = 0; i < t->children.size(); i++) {
      printf("Has child %p\n", t->children[i]);
    }

    for (i = 0; i < t->children.size(); i++) {
      nodes.push(t->children[i]);
    }
  }
}

/* Always call this on the root node.
   If you call this from another node,
   you're a dick. */
void QTnode::calc_global_accel()
{
  int i;
  QTnode *t;
  std::queue <QTnode *> nodes;

  nodes.push(this);

  /* Perform a breadth-first traversal
     through all particles. */

  while (!nodes.empty()) {
    t = nodes.front();
    nodes.pop();

    if (t->children.empty()) {
      for (std::list <Particle>::iterator p = t->particles.begin(); p != t->particles.end(); p++) {
        (*p).accel = calc_accel(*p);
        // printf("Particle has accel <%lf, %lf>\n", (*p).accel.x, (*p).accel.y);
      }
    } else {
      for (i = 0; i < t->children.size(); i++) {
        nodes.push(t->children[i]);
      }
    }
  }
}

/* Call this on root node pls. */
void QTnode::move_shit()
{
  QTnode *t;
  std::queue <QTnode *> nodes;
  std::list <Particle> tmp;

  nodes.push(this);

  while (!nodes.empty()) {
    t = nodes.front();
    nodes.pop();

    if (!t->children.empty()) {
      for (int i = 0; i < t->children.size(); i++) {
        nodes.push(t->children[i]);
      }
    } else {
      for (std::list <Particle>::iterator p = t->particles.begin(); p != t->particles.end(); p++) {
        tmp.push_back(phys_move_particle(*p));
      }
      t->particles.clear();
      t->psize = 0;
    }
  }

  for (std::list <Particle>::iterator p = tmp.begin(); p != tmp.end(); p++) {
    insert(*p);
  }
}

/* Calculate acceleration for p,
   looking through QT with BFS.  */
f2 QTnode::calc_accel(Particle p)
{
  double d, s;
  std::queue <QTnode *> nodes;
  QTnode *t;

  nodes.push(this);

  // YOLO
  f2 accel = (f2) {0.0, 0.0};

  while (!nodes.empty()) {
    t = nodes.front();
    nodes.pop();

    d = f2_norm(f2_minus(p.pos, t->get_centroid()));
    s = t->x.lim - t->x.beg;

    if (s / d < THETA) {
      /* Far enough away to use quad as point. */
      /* Do it and set accel. */
      accel = f2_add(accel, f2_mult(phys_get_force(f2_minus(p.pos, t->get_centroid())), t->get_num_particles()));
    } else {
      /* Quad is too close. If leaf, then do particles. */
      if (t->children.empty()) {
        for (std::list <Particle>::iterator pp = t->particles.begin(); pp != t->particles.end(); pp++) {
          /* Compute distance shit, set accel. */
          accel = f2_add(accel, phys_get_force(f2_minus(p.pos, (*pp).pos)));
        }
      } else {
        /* Not leaf, and quad isn't good enough.
           Add children for EXAMINATION. */
        for (int i = 0; i < children.size(); i++) {
          nodes.push(t->children[i]);
        }
      }
    }
  }

  // YOLO again
  return accel;
}

/* Centroid is just center of quad (for now). */
f2 QTnode::get_centroid()
{
  return (f2) {(x.lim + x.beg) / 2.0, (y.lim + y.beg) / 2.0};
}

/* Always insert into root (first).
   If you insert into another node,
   you're a dick. */
bool QTnode::insert(Particle p)
{
  int i;
  bool inserted;

  inserted = false;

  if (in_bounds(p)) {
    if (children.size() == 0) {
      /* If we are at a leaf node. Just insert. */
      particles.push_back(p);
      psize++;
      inserted = true;
    } else {
      /* Otherwise, see if you can store in a child. */
      for (i = 0; i < children.size(); i++) {
        if (children[i]->in_bounds(p)) {
          inserted = children[i]->insert(p);
          break;
        }
      }
    }
  } else {
    /* You are at a singularity. YOLO. */
    return false;
  }

  /* If you inserted a particle into your child,
     then re-calculate your number of particles. */
  if (inserted) {
    recalc_num_particles();
  }

  return inserted;
}

bool QTnode::in_bounds(Particle p)
{
  return (p.pos.x > x.beg && p.pos.x < x.lim) && (p.pos.y > y.beg && p.pos.y < y.lim);
}

int QTnode::get_num_particles()
{
  return num_particles;
}

void QTnode::recalc_num_particles()
{
  int i;

  if (children.empty()) {
    num_particles = psize;
  } else {
    num_particles = 0;

    for (i = 0; i < children.size(); i++) {
      num_particles += children[i]->get_num_particles();
    }
  }
}

QTnode::QTnode(QTnode *parent, double xbeg, double ybeg, double step, int height)
{
  this->parent = parent;

  this->x.beg = xbeg;
  this->x.lim = xbeg+step;
  this->y.beg = ybeg;
  this->y.lim = ybeg+step;

  this->num_particles = 0;
  this->height = height;

  this->psize = 0;
}

QTnode *init_tree(int height, QTnode *parent)
{
  int i, j;
  double xbeg, ybeg, step;
  QTnode *t;

  if (height >= 0) {
    if (!parent) {
      t = new QTnode(parent, 0.0, 0.0, SIZE, height);
      init_tree(height-1, t);
    } else {
      step = (parent->x.lim - parent->x.beg) / 2.0;
      j = 0;
      for (ybeg = parent->y.beg; j < 2; ybeg += step) {
        i = 0;
        for (xbeg = parent->x.beg; i < 2; xbeg += step) {
          t = new QTnode(parent, xbeg, ybeg, step, height);
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

  parent->children.clear();
  parent->particles.clear();

  delete parent;

  for (i = 0; i < children.size(); i++) {
    destroy_tree(children[i]);
  }
}
