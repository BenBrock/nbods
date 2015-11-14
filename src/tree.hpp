#include <stdlib.h>
#include <stdio.h>
#include "phys.hpp"

typedef struct {
  double beg;
  double lim;
} Bounds;

typedef struct {
  Particle p;
  Particle *next;
} Plist;

typedef struct {
  QTnode *children[4];
  QTnode *parent;
  Plist *pl;
  Bounds x, y;
} QTnode;

QTnode *init_tree(int tree_height);
void destroy_tree(QTnode *root);

QTnode *init_tnode(QTnode *parent);
QTnode *init_children(QTnode *parent);
void destroy_tnode(QTnode *t);

void print_tree(QTnode *root);
void set_lim(QTnode *t);
