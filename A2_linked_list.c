#include <stdlib.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include "A2_functions.h"
#include "A2_linked_list.h"

//Creates and intializes a new file_node 
file_node *new_file_node()
{
    //Allocates memory needed for a new node
	file_node *new_node = NULL;
	new_node = malloc(sizeof(file_node));
	
    //Initializes all the values 
	strcpy(new_node->PID,"");
	strcpy(new_node->FD,"");
    new_node->inode_num = 0;
    strcpy(new_node->Filename,"");

	new_node->next = NULL;
    return(new_node);    
}

//Inserting a new node to a linked list at the tail
void insert_at_tail(file_node *head, char insert_PID[STR_LEN], unsigned long insert_inode_num, char insert_FD[STR_LEN], char insert_Filename[STR_LEN]){

    //Creates a new empty file_node
    file_node *new_node = new_file_node();

    //Checks if memory allocation was successful
    if(new_node == NULL) {
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(-1);
    }

    //Sets the data that was passed through the function into the  node created above
    strcpy(new_node->PID,insert_PID);
    new_node->inode_num = insert_inode_num;
	strcpy(new_node->FD,insert_FD);
	strcpy(new_node->Filename,insert_Filename);
    new_node->next = NULL; 

    //Check if the head is empty, if it is, add the new node to the head
    if(head->next == NULL) {
        head->next = new_node;

    }

    //Finds the tail of the link list, then adds the node to the end
    else {
        file_node *current = head;
        while (true) {
            if(current->next == NULL) {
                current->next = new_node;
                //printf("added at tail\n");
                break;
            }
            current = current->next;
        }
    }
}

//Iterates through the entire list, and frees every node, effectively deleting the entire linked list
void delete_entire_list(file_node *head){
	file_node *a = NULL;
	file_node *b = NULL;
	
	a = head;

	while(a != NULL) {
		b = a->next;
		free(a);
		a = b;
	}
}
