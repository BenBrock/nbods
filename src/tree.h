#include "phys.h"

#define BRANCHING_FACTOR 4

typedef struct {
  Particle p;
  Particle *next;
} Plist;

typedef struct {
  Tnode *children[BRANCHING_FACTOR];
  Tnode *parent;
  Plist *pl;
} Tnode;

Tnode *init_tree(int tree_height);
void destroy_tree(Tnode *t);

Tnode *init_tnode(Tnode *parent);
void destroy_tnode();
