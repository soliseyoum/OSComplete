#include "BENSCHILLIBOWL.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>
/*
bool IsEmpty(BENSCHILLIBOWL* bcb);
bool IsFull(BENSCHILLIBOWL* bcb);
void AddOrderToBack(Order **orders, Order *order);
*/
MenuItem BENSCHILLIBOWLMenu[] = { 
    "BensChilli", 
    "BensHalfSmoke", 
    "BensHotDog", 
    "BensChilliCheeseFries", 
    "BensShake",
    "BensHotCakes",
    "BensCake",
    "BensHamburger",
    "BensVeggieBurger",
    "BensOnionRings",
};
int BENSCHILLIBOWLMenuLength = 10;





/* Select a random item from the Menu and return it */
MenuItem PickRandomMenuItem() {
    int menu_item_index = (rand() % BENSCHILLIBOWLMenuLength);
    //MenuItem selected_item;
    //selected_item = BENSCHILLIBOWLMenu[menu_item_index];
    return BENSCHILLIBOWLMenu[menu_item_index];
}






/* Allocate memory for the Restaurant, then create the mutex and condition variables needed to instantiate the Restaurant */

BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders) {
    /*
typedef struct Restaurant {
    Order* orders;
    int current_size;
    int max_size;
    int next_order_number;
    int orders_handled;
	int expected_num_orders;
    pthread_mutex_t mutex;
    pthread_cond_t can_add_orders, can_get_orders;
} BENSCHILLIBOWL;
*/
    BENSCHILLIBOWL *bcb = (BENSCHILLIBOWL*) malloc(sizeof(BENSCHILLIBOWL)); //ALLOCATE MEM
    bcb->orders= NULL; //populate restaurant
    bcb->current_size= 0;
    bcb->max_size= max_size;
    bcb->next_order_number= 1;
    bcb->orders_handled= 0;
    bcb->expected_num_orders= expected_num_orders;

    pthread_mutex_init(&(bcb->mutex), NULL); //populate mutex var
    pthread_cond_init(&(bcb->can_add_orders), NULL); // populate condition vars
    pthread_cond_init(&(bcb->can_get_orders), NULL); 



    printf("Restaurant is open!\n");
    return bcb; // return restaurant to main
}




/* check that the number of orders received is equal to the number handled (ie.fullfilled). Remember to deallocate your resources */

void CloseRestaurant(BENSCHILLIBOWL* bcb) {

    if (bcb->orders_handled != bcb->expected_num_orders) {
        // if the number of orders received is not equal to the number handled
        fprintf(stderr, "The number of orders received is not equal to the number handled\n");
        exit(1);
    }
    // deallocate resources if the number of orders received is equal to the number handled
    free(bcb); //free mem
    pthread_mutex_destroy(&(bcb->mutex)); // destroy mutex var
    
    printf("Restaurant is closed!\n");
}






/* add an order to the back of queue */
int AddOrder(BENSCHILLIBOWL* bcb, Order* order) {
    // lock thread
    pthread_mutex_lock(&(bcb->mutex)); 
  
    // wait to add  if restaurant is full of orders
    while (bcb->current_size == bcb->max_size) { 
        pthread_cond_wait(&(bcb->can_add_orders), &(bcb->mutex));
    }
    
    // put order in queue
    order->order_number = bcb->next_order_number; 
    Order **orders = &(bcb->orders);
    //AddOrderToBack(&(bcb->orders), order);
    if (*orders == NULL) {  // add order if order queue is empty
        *orders = order; 
    }
    else {  // if queue isnt empty
        Order *temp_order = *orders;
        while (temp_order->next) { // move new order through the orders queue 
            temp_order = temp_order->next;
        }
        temp_order->next = order; //and set it at the end
        //free(temp_order);
    }

    bcb->next_order_number += 1; 
    bcb->current_size += 1;
    
    // allow orders and unlock
    pthread_cond_broadcast(&(bcb->can_get_orders));
    pthread_mutex_unlock(&(bcb->mutex));
    
    return order->order_number; //return order num
}




/* remove an order from the queue */
Order *GetOrder(BENSCHILLIBOWL* bcb) {
    // lock thread
    pthread_mutex_lock(&(bcb->mutex)); 
    
    // check if order queue is populated
    while (bcb->current_size == 0) {
        // if queue is empty and expected number of orders have been handled, don't take an order
        if (bcb->orders_handled >= bcb->expected_num_orders) {
            pthread_mutex_unlock(&(bcb->mutex));
            return NULL;
        }
        // if queue is empty and not all expected orders have been handled
        pthread_cond_wait(&(bcb->can_get_orders), &(bcb->mutex)); //wait until mutex unlocked to take more
    }

    // if order is populated 
    Order *order = bcb->orders;
    bcb->orders = bcb->orders->next; // get first order from queue
    bcb->current_size -= 1; 
    bcb->orders_handled += 1; 

    // allow orders and unlock
    pthread_cond_broadcast(&(bcb->can_add_orders));
    pthread_mutex_unlock(&(bcb->mutex));

    return order; //return first order 

}
/*
// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(BENSCHILLIBOWL* bcb) {
  return false;
}

bool IsFull(BENSCHILLIBOWL* bcb) {
  return false;
}

 this methods adds order to rear of queue 
void AddOrderToBack(Order **orders, Order *order) {}
*/
