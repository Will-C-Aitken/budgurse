
// data entry for purchase or reimbursement
typedef struct entry {
    char* name;
    float amount;
    char* category;
    char* subcategory;
    char* note;
} entry_t;


// node representing an entry in doubly linked list
typedef struct entry_node {
    struct entry_node* next;
    struct entry_node* prev;
    entry_t* data;
} entry_node_t;


// doubly linked list for holding entries
typedef struct entry_list {
    entry_node_t* head;
    entry_node_t* tail;
    int num_nodes;
} entry_list_t;


// entry declarations
entry_t* init_entry(char* name, float amount, char* category, 
    char* subcategory, char* note);
void free_entry(entry_t* e);

// entry node declarations
entry_node_t* init_entry_node(entry_t* e);

// entry list declarations
entry_list_t* init_entry_list();
void append_to_tail(const entry_list_t* el, const entry_node_t* nd);
void free_head(const entry_list_t* el);
void list_to_string(const entry_list_t* el);
