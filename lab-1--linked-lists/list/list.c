// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include "list.h"
/* Functions for allocating and freeing lists. By using only these functions,
 * the user should be able to allocate and free all the memory required for
 * this linked list library. */
list_t *list_alloc() { 
 list_t *my_list = (list_t *) malloc(sizeof(list_t));
  return my_list; 
}


void list_free(list_t *l) {
  node_t * current = l->head;
  while (current != NULL) {
  free(current);
    current = current->next;
  }
}


/* Prints the list in some format. */
void list_print(list_t *l) {
  node_t * current = l->head;
  while (current != NULL) {
        printf("%d\n", current->value);
        current = current->next; 
    }
}
  
/* Returns the length of the list. */
int list_length(list_t *l) { 
  int count = 0;
  node_t * current = l->head;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count; }

/* Method for adding to the back of list. */
void list_add_to_back(list_t *l, elem value) {
  node_t * current = l->head;
  while (current->next != NULL) {
    current = current->next;
  }

    /* now we can add a new variable */
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->value = value;
    current->next->next = NULL;
}
  

 /* Method for adding to the front of list. */
void list_add_to_front(list_t * l, elem value) { 
  
  node_t * new_head = (node_t *) malloc(sizeof(node_t));
  new_head->value = value;
  new_head->next = l->head;
  l->head = new_head; 
}

/* Method for adding to the list at index. */ 
void list_add_at_index(list_t *l, elem value, int index) {
  int i = 0;
  node_t * current = l->head;

    if (index == 0) { // if index is zero, add to front
  node_t * new_head = (node_t *) malloc(sizeof(node_t));
  new_head->value = value;
  new_head->next = l->head;
  l->head = new_head;   
    } else { //if index isn't zero, add at index
  /* get the node before index */
  for (i = 0; i < index-1; i++) {
    // check if a node exists at the index
    if (current->next == NULL) {
            //exit(-1);
          break;
        }else { 
        current = current->next;}
    }
// make sure node exists at index
    if (current->next != NULL) {
node_t * new_node = (node_t *) malloc(sizeof(node_t));
new_node->value = value;
new_node->next = current->next;
current->next = new_node;  }
}}



 /* Method for removing from the back of list. Returns the removed element. */
elem list_remove_from_back(list_t *l) { 
  node_t * current = l->head;
  int retval = 0;
      if (current->next == NULL) {
        retval = current->value;
        free(current);
        return retval;
    }
  while (current->next->next != NULL) {
        current = current->next;
    }

    /* now current points to the second to last item of the list, so let's remove current->next */
    retval = current->next->value;
    free(current->next);
    current->next = NULL;
    return retval;
  }
  

/* Method for removing from the front of list. Returns the removed element. */
elem list_remove_from_front(list_t *l) { 
    node_t * current = l->head;
    node_t * next_node = NULL;
    int retval = -1;
    if (current == NULL) {
        return -1;
    }
   next_node = current->next;
   retval = current->value;
   current->value = next_node->value;  
   current->next = next_node->next;    
   free(next_node);
    return retval;
   // alternate algorithm 
   /* node_t * current = l->head;
    node_t * next_node = NULL;
    int retval = -1;
    if (current == NULL) {
        return -1;
    }
    next_node = current->next;
    retval = current->value;  
    l->head = l->head->next; 
        return retval;
    */
}
  

 /* Method for removing from the list at index. Returns the removed element. */  
elem list_remove_at_index(list_t *l, int index) { 
  int i = 0;
    int retval = -1;
    node_t * current = l->head;
    node_t * temp_node = NULL;

    if (index == 0) {
    node_t * next_node = NULL;
    next_node = current->next;
    retval = current->value;
    free(current);
    current = next_node;
    return retval; 
    }else{

    for (i = 0; i < index-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }

    if (current->next == NULL) {
        return -1;
    }

    temp_node = current->next;
    retval = temp_node->value;
    current->next = temp_node->next;
    free(temp_node);

    return retval;
}}

  
/* Checks to see if the given element exists in the list. */  
bool list_is_in(list_t *l, elem value) { 
  node_t * head = l->head;
  node_t *previous, *current;

    if (head == NULL) {
        return false;
    }
    if ((head)->value == value) {
        return true;
    }
    previous = head;
    current = (head)->next;
    while (current) {
        if (current->value == value) {
            return true;
        }
        previous = current;
        current  = current->next;
    }
  return false; 
}
  
  
/* Returns the element at the given index. */
elem list_get_elem_at(list_t *l, int index) { 
  int i = 0;
  int retval = -1;
  node_t * current = l->head;

    if (index == 0) {
    retval = current->value;
    return retval; 
    }
while (current != NULL) {
        if (i == index) {
          retval = current->value;
          return retval; }
        i++;
        current = current->next;
    }
    return retval; 
}
  
  
/* Returns the index at which the given element appears. */  
int list_get_index_of(list_t *l, elem value) { 
  node_t * current = l->head;
  int i = 0;
  while (current != NULL) {
    if ((current->value) == value) {
      return i;}
    i++;
    current = current->next;
  }
  return -1;}

