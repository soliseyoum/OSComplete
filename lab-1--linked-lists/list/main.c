#include <stdio.h>
#include<stdlib.h>
#include "list.h"

int main() {
  printf("Tests for linked list implementation\n");
  //create list and free it
      list_t *test_list = list_alloc();
      list_free(test_list);
  // create list and add first node
      test_list = list_alloc();
      test_list->head = (node_t *) malloc(sizeof(node_t));
  // set values to linked list and build the list
      test_list->head->value = 1;
      test_list->head->next = (node_t *) malloc(sizeof(node_t));
      test_list->head->next->value = 2;
      test_list->head->next->next = (node_t *) malloc(sizeof(node_t));
      test_list->head->next->next->value = 3;
      test_list->head->next->next->next = (node_t *) malloc(sizeof(node_t));
      test_list->head->next->next->next->value = 4;
      test_list->head->next->next->next->next = NULL;
    
         printf("Linked list at start:\n");
     list_print(test_list);

     // find length
    int length = list_length(test_list);
    printf("Length of linked list at start: %d\n", length);
    // add new node to end
    list_add_to_back(test_list, 5);
    printf("We've added a new item to the back! Updated linked list: \n");
     list_print(test_list);
    // add new node to front
    list_add_to_front(test_list, 0);
    printf("We've added a new item to the front! Updated linked list: \n");
     list_print(test_list);   
// add new node at third index
list_add_at_index(test_list, 7, 3);
printf("We've added a new item in the third index! Updated linked list: \n");
     list_print(test_list);  
  
// add new node at 0th index
list_add_at_index(test_list, 9, 0);
printf("We've added a new item in the 0th index! Updated linked list: \n");
     list_print(test_list); 
  
// add new node at 10th index 
list_add_at_index(test_list, 8, 10);
printf("We've added a new item in the 10th index(doesn't exist)! Updated linked list: \n");
     list_print(test_list); 
  
//remove the last node
list_remove_from_back(test_list);
printf("We've removed the last node! Updated linked list: \n");
     list_print(test_list);
//remove the first node
list_remove_from_front(test_list);
printf("We've removed the first node! Updated linked list: \n");
     list_print(test_list);

// remove node at third index
list_remove_at_index(test_list, 3);
printf("We've removed the node in the third index! Updated linked list: \n");
     list_print(test_list); 
  
// remove node at 10th index
list_remove_at_index(test_list, 10);
printf("We've removed the node in the 10th index (doesn't exist)! Updated linked list: \n");
     list_print(test_list); 
 
// checks to see if 3 is in the linked list
if (list_is_in(test_list, 3) == true) {
printf("\n3 is in the linked list! \n");
}
  else {
    printf("\n3 is NOT in the linked list! \n");
  }

// checks to see if 7 is in the linked list (is not)
if (list_is_in(test_list, 7) == true) {
printf("\n7 is in the linked list! \n");
}
  else {
    printf("\n7 is NOT in the linked list! \n");
  } 
  
// returns element at 2nd index
int element = list_get_elem_at(test_list, 2);
printf("The element at the 2nd index: %d\n", element);

// returns element at 0th index
element = list_get_elem_at(test_list, 0);
printf("The element at the 0th index: %d\n", element);

// returns element at 5th index
element = list_get_elem_at(test_list, 5);
printf("The element at the 5th index (doesn't exist): %d\n", element);
  
// get index where 3 appears
int index = list_get_index_of(test_list, 3);
printf("The index where 3 appears: %d\n", index);
  
// get index where 22 appears (does not)
index = list_get_index_of(test_list, 22);
printf("The index where 22 appears (does not appear): %d\n", index);
  
  return 0;
}
