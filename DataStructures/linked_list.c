#include <stdio.h>
#include <stdlib.h>

/**
 * Inspiration using double pointers :
 * http://www.alexonlinux.com/rethinking-linked-list-insertion
 */

typedef int T;

typedef struct node {
  T value;
  struct node * next;
} linked_list_t;

linked_list_t * linked_list_alloc_node(T value) {
  linked_list_t * node = (linked_list_t *)malloc(sizeof(linked_list_t));
  node->value=value;
  node->next=NULL;
  return node;
}

int linked_list_add(linked_list_t ** this, T value) {
  while((*this))
    this=&((*this)->next);
  (*this) = linked_list_alloc_node(value);
}

void linked_list_traversal(linked_list_t ** this, void (*callback)(linked_list_t *)) {
  while((*this)) {
    callback(*this);
    this=&((*this)->next);
  }
}

void my_callback(linked_list_t * current_node) {
  fprintf(stderr, "%d\n", current_node->value);
}

void dump(linked_list_t ** this) {
  linked_list_traversal(this,my_callback);
}

/**
 * Linked List structure after insertion
 *  5->15->25->35->135
 */
int main(int argc, char const *argv[]) {
  linked_list_t * linked_list=NULL;

  linked_list_add(&linked_list,5);
  linked_list_add(&linked_list,15);
  linked_list_add(&linked_list,25);
  linked_list_add(&linked_list,35);
  linked_list_add(&linked_list,135);

  dump(linked_list);

  return 0;
}
