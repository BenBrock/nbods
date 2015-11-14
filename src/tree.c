#include <stdlib.h>
#include <stdio.h>

Tnode *init_tree(int tree_height)
{
}

void destroy_tree(Tnode *t)
{
  int i;
  for (i = 0; i < BRANCHING_FACTOR; i++) {
  }
}

Tnode *init_tnode(Tnode *parent)
{
  int i;
  Tnode *t;

  t = malloc(sizeof(Tnode));

  for (i = 0; i < BRANCHING_FACTOR; i++) {
    t->children[i] = NULL;
  }

  t->parent = parent;
  t->pl = NULL;

  return t;
}

void destroy_tnode(Tnode *t)
{
  Plist *p, *tmp;

  p = t->pl;

  while (p) {
    tmp = p->next;
    free(p);
    p = tmp;
  }

  free(t);
}
