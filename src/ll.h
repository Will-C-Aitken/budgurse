typedef struct ll_item ll_item {
    ll_item* next;
    void* data;
}

typedef struct ll ll {
    ll_item* head;
}

ll_item* ll_item_init(void* data);
