//THIS IS THE HEADER FILE FOR ALL THE LINKED LIST IMPLEMENTATION USED IN B09_A2.C (the main project file)
#ifndef LINKED_LIST
#define LINKED_LIST
#include "A2_functions.h"
#define STR_LEN 1024

//Typedef of file_node
typedef struct file_node{
    char PID[STR_LEN];
    unsigned long inode_num;
    char FD[STR_LEN];
    char Filename[STR_LEN];
    struct file_node *next;
}file_node;

//Linked List Functions 
file_node *new_file_node();
void insert_at_tail(file_node *, char [STR_LEN], unsigned long, char [STR_LEN], char [STR_LEN]);
void delete_entire_list(file_node *);

#endif 