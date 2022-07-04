#include "entries.h"

// ---------------------------------------------------------------------------
// Entry Definitions
// ---------------------------------------------------------------------------

entry_t* init_entry(char* name, time_t date, float amount, 
	char* category, char* subcategory, char* note) {

    entry_t* e = malloc(sizeof(entry_t));

    e->date = date;
    e->amount = amount;
    e->name = strdup(name);
    e->category = strdup(category);
    e->subcategory = strdup(subcategory);
    e->note = strdup(note);

    return e;
}   


void free_entry(entry_t* e) {

    free(e->name);
    free(e->category);
    free(e->subcategory);
    free(e->note);
    free(e);
    return;

}

char* entry_to_sql_insert(entry_t* e) {

    char* sql_to_append = "INSERT INTO ENTRIES VALUES ('";

    char* sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);


    sql_to_append = "', ";
    append_to_sql(&sql, sql_to_append, e->name);

    sql_to_append = ", ";
    char date_str[10];
    sprintf(date_str, "%ld", e->date);
    append_to_sql(&sql, sql_to_append, date_str);

    sql_to_append = ", '";
    char amount_str[10];
    sprintf(amount_str, "%0.2f", e->amount);
    append_to_sql(&sql, sql_to_append, amount_str);

    sql_to_append = "', '";
    append_to_sql(&sql, sql_to_append, e->category);
    append_to_sql(&sql, sql_to_append, e->subcategory);

    sql_to_append = "');";
    append_to_sql(&sql, sql_to_append, e->note);

    return sql;
}


void append_to_sql(char** cur_sql, const char* sql_to_append, 
	const char* data_to_append) {
    
    *cur_sql = realloc(*cur_sql, 1 + 
	    (sizeof(char) * (strlen(sql_to_append) +
			     strlen(*cur_sql) +
			     strlen(data_to_append))));
    strcat(*cur_sql, data_to_append);
    strcat(*cur_sql, sql_to_append);
    return;
}


// ---------------------------------------------------------------------------
// Entry Node Definitions
// ---------------------------------------------------------------------------

entry_node_t* init_entry_node(entry_t* e) {

    entry_node_t* nd = malloc(sizeof(entry_node_t));

    nd->next = NULL;
    nd->prev = NULL;
    nd->data = e;

    return nd;
    
}


void free_entry_node(entry_node_t* e) {

    free_entry(e->data);
    free(e);
    return;

}


// ---------------------------------------------------------------------------
// Entry List Definitions
// ---------------------------------------------------------------------------

entry_list_t* init_entry_list() {

    entry_list_t* el = malloc(sizeof(entry_list_t));

    el->head = NULL;
    el->tail = NULL;
    el->num_nodes = 0;
    
    return el;
}


void append_to_tail(entry_list_t* el, entry_node_t* nd) {

    el->num_nodes++;
    
    if(el->num_nodes == 1) {
	el->head = nd;
	el->tail = nd;
	return;
    }

    el->tail->next = nd;
    nd->prev = el->tail;
    el->tail = nd;
}


void free_head(entry_list_t* el) {

    if(el->num_nodes == 0)
	return;


    entry_node_t* temp = el->head;
    el->head = temp->next;
    el->num_nodes--;
    
    free_entry_node(temp);

    if(el->num_nodes == 0)
	return;

    el->head->prev = NULL;
    return;


}


void free_list(entry_list_t* el) {
    while(el->num_nodes > 0)
	free_head(el);
    free(el);
    return;
}



void list_to_string(const entry_list_t* el) {

    if(el->head == NULL) {
	printf("Empty List\n"); 
	return;
    }

    entry_node_t* cur = el->head;

    do {
	printf("%s %ld %0.2f %s %s %s\n", cur->data->name, 
		cur->data->date, cur->data->amount, 
		cur->data->category, cur->data->subcategory, 
		cur->data->note);
	cur = cur->next;
    } while(cur != NULL); 
}


entry_t* get_tail(entry_list_t* el) {
    return(el->tail->data);
}
