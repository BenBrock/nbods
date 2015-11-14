#include <cstdlib>
#include <cstdio>
#include "tree.hpp"

int main(int argc, char **argv)
{
  QTnode *tree;

  tree = init_tree(2, NULL);

  tree->Print();

  destroy_tree(tree);

  return 0;
}
