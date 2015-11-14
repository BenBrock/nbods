#include <cstdlib>
#include <cstdio>
#include <queue>

void print_tree(QTnode *root)
{
  int i;
  int node_no;
  QTnode *t;
  std::queue <QTnode *> nodes;

  nodes.push(root);
  node_no = 0;

  while (!nodes.empty()) {
    t = nodes.front();
    nodes.pop();
    printf("Node %d, %p\n", node_no++, t);
    printf("Has parent %p\n", t->parent);
    printf("Has x limits %lf -> %lf\n", t->x->beg, t->x->lim);
    printf("Has y limits %lf -> %lf\n", t->y->beg, t->y->lim);
    for (i = 0; i < 4; i++) {
      if (t->children[i]) {
        nodes.push(t->children[i]);
      }
    }
  }
}

QTnode *init_tree(QTnode *root, int tree_height)
{
  int i;
  QTnode *t;

  if (!root) {
    root = init_tnode(NULL);
  }

  init_children(root);

  if (tree_height > 0) {
    for (i = 0; i < 4; i++) {
      init_tree(root->children[i], tree_height-1);
    }
  }

  return root;
}

void destroy_tree(QTnode *root)
{
  int i;

  for (i = 0; i < 4; i++) {
    if (root->children[i]) {
      destroy_tree(root->children[i]);
    }
  }

  destroy_tnode(root);
}

QTnode *init_tnode(QTnode *parent)
{
  int i;
  QTnode *t;

  t = malloc(sizeof(QTnode));

  for (i = 0; i < 4; i++) {
    t->children[i] = NULL;
  }

  t->parent = parent;
  t->pl = NULL;

  if (t->parent == NULL) {
    t->xlim = 1.0;
    t->ylim = 1.0;
  }

  return t;
}

QTnode *init_children(QTnode *parent)
{
  int k;
  double i, j;
  double step;
  Bounds x, y;

  step = (parent->xlim - parent->xbeg) / 2.0;

  k = 0;

  for (j = parent->ybeg; k < 4; j += step) {
    for (i = parent->xbeg; k < 4; i += step) {
      parent->children[k] = init_tnode(parent);
      set_lim(parent->children[k], i, j, step);
      k++;
    }
  }

  return parent;
}

void destroy_tnode(QTnode *t)
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

void set_lim(QTnode *t, double xbeg, double ybeg, double step)
{
  t->x->beg = xbeg;
  t->x->lim = xbeg+step;
  t->y->beg = ybeg;
  t->y->lim = ybeg+step;
}
