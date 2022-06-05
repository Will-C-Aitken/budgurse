#include "ll.h"

ll_item* ll_item_init(void* data) {
    ll_item* i = malloc(sizeof(ll_item));

    if(i) {
	i->next = NULL;
	i->data = data;
    }

    return i;
}
    
void ll_item_print(ll_item* i) { 
    if(i) 
	printf(int*(
