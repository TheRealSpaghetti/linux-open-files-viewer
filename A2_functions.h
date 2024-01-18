#ifndef FUNCTIONS
#define FUNCTIONS

#include "A2_linked_list.h"
#define STR_LEN 1024

//THIS IS THE HEADER FILE FOR ALL THE FUNCTIONS USED IN B09_A2.C (the main project file)
file_node *pick_pid(file_node *, char *);
void threshold_print(file_node *, unsigned long);
void insert_data(file_node *);
void vnode_print(file_node *);
void system_wide_print(file_node *);
void per_process_print(file_node *);
void composite_print(file_node *);
void write_in_text(file_node *);
void write_in_binary(file_node *);

#endif 