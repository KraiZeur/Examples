#include <stdio.h>
#include <stdlib.h>

/**
 * Inspiration using double pointers :
 * http://www.alexonlinux.com/rethinking-linked-list-insertion
 */

typedef int T;

typedef struct node {
  T value;
  struct node * left;
  struct node * right;
} binary_tree_t;

binary_tree_t * binary_tree_alloc_node(T value) {
  binary_tree_t * node = (binary_tree_t *)malloc(sizeof(binary_tree_t));
  (*node).value=value;
  (*node).right=(*node).left=NULL;
  return node;
}

void binary_tree_add(binary_tree_t ** this, T value) {
  while((*this))
    this=((value>=(*this)->value)?&((*this)->right):&((*this)->left));
  (*this) = binary_tree_alloc_node(value);
}

/**
 * Return 1 if the node is found and deleted 0 else.
 */
int binary_tree_delete(binary_tree_t ** this, T value) {
  while ((*this) && this)
  {
    if (value == (*this)->value)
    {
      if ((*this)->right)
      {
        binary_tree_t* current_node=(*this);
        (*this) = (*this)->right;
        printf("Found as Right\n");
        free(current_node); // Specific free should be use

        return 1;
      }
      else if ((*this)->left)
      {
        binary_tree_t* current_node=(*this);
        (*this) = (*this)->left;
        printf("Found as Left\n");
        free(current_node); // Specific free should be use

        return 1;
      }
      else
      {
        printf("Found as Child\n");
        free((*this));

        return 1;
      }
    }
    else
    {
      this = ((value > (*this)->value) ? &((*this)->right) : &((*this)->left));
    }
  }

  return 0;
}

/** Using a stack can prevent recursive algorithm */
void binary_tree_preorder(binary_tree_t ** this, void (*callback)(binary_tree_t *)) {
  if ((*this)==NULL) {
    return;
  }

  callback((*this));
  binary_tree_preorder(&((*this)->left),callback);
  binary_tree_preorder(&((*this)->right),callback);
}

void binary_tree_inorder(binary_tree_t ** this, void (*callback)(binary_tree_t *)) {
  if ((*this)==NULL) {
    return;
  }

  binary_tree_inorder(&((*this)->left),callback);
  callback((*this));
  binary_tree_inorder(&((*this)->right),callback);
}

void binary_tree_postorder(binary_tree_t ** this, void (*callback)(binary_tree_t *)) {
  if ((*this)==NULL) {
    return;
  }

  binary_tree_postorder(&((*this)->left),callback);
  binary_tree_postorder(&((*this)->right),callback);
  callback((*this));
}

/**
 * Depth could be stored inside node to improve performances.
 */
int binary_tree_depth(binary_tree_t ** this) {
  if((*this)==NULL) {
    return 0;
  }

  int left_depth =binary_tree_depth(&(*this)->left);
  int right_depth=binary_tree_depth(&(*this)->right);

  return ((left_depth>right_depth)?left_depth:right_depth)+1;
}

void my_callback(binary_tree_t * current_node) {
  fprintf(stderr, "%d\n", current_node->value);
}

void binary_tree_dump(binary_tree_t ** this) {
  binary_tree_preorder(this, my_callback);
}

/**
 * Tree structure
 *
 *     [[10]]
 *  [5]      [20]
 *      [15]      [30]
 *                     [45]
 *                [37]
 */
int main(int argc, char const *argv[]) {
  binary_tree_t * binary_tree=NULL;

  binary_tree_add(&binary_tree,10);
  binary_tree_add(&binary_tree,20);
  binary_tree_add(&binary_tree,30);
  binary_tree_add(&binary_tree,5);
  binary_tree_add(&binary_tree,15);
  binary_tree_add(&binary_tree,45);
  binary_tree_add(&binary_tree,37);

  printf("Address :: %p\n", &binary_tree);
  printf("Address :: %p\n", binary_tree);
  printf("Address :: %p\n", &(binary_tree->right));
  printf("Address :: %p\n", (*binary_tree).right);

  printf("Binary Tree Depth : %d\n", binary_tree_depth(&binary_tree));

  puts("Binary Tree Pre-Order traversal");
  binary_tree_preorder(&binary_tree, my_callback);
  puts("Binary Tree In-Order traversal");
  binary_tree_inorder(&binary_tree, my_callback);
  puts("Binary Tree Post-Order traversal");
  binary_tree_postorder(&binary_tree, my_callback);

  puts("Dumping Binary Tree");
  binary_tree_dump(&binary_tree);


  binary_tree_delete(&binary_tree, 10);

  puts("Binary Tree In-Order traversal");
  binary_tree_inorder(&binary_tree, my_callback);

  return 0;
}
