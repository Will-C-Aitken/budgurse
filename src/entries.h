
// data entry for purchase or reimbursement
typedef struct entry {
    char* name;
    float amount;
    char* category;
    char* subcategory;
    char* note;
} entry_t;


// node representing an entry in doubly linked list
typedef struct entry_list_item {
    entry_node_t* next;
    entry_node_t* prev;
    entry_t* data;
} entry_node_t;


// doubly linked list for holding entries
typdef struct entry_list {
    entry_node_t* head;
    entry_node_t* tail;
    int num_nodes;
} entry_list_t;

// entry declarations
entry_t* init_entry(char* name, float amount, char* category, 
    char* subcategory, char* note);
void free_entry(entry_t* e);