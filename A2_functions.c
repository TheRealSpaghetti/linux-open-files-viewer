#include <sys/sysinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <utmp.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include "A2_functions.h"
#include "A2_linked_list.h"


//Reads the data in the linked list, and prints it in the composite format
void composite_print(file_node *head){

    //Initalizes variables needed
    int count = 0;
	file_node *printer_boi = NULL;
	printer_boi = head;
	
    //Prints the table header columns
    printf("PID         INODE       FD    Filename\n");
    printf("======================================\n");

    //Iterates through each node in the linked list and prints all the data in the composite table format
	while(printer_boi != NULL) {                  //Checks if the linked list is null
        if(count != 0){                           //Skip the first node (the head doesn't contain any information)

            //Prints the information contained in the node in the specified format
            printf("%s   %lu     %s    %s    \n", printer_boi->PID, printer_boi->inode_num, printer_boi->FD, printer_boi->Filename);
        }
        count++;
		printer_boi = printer_boi->next;          //Continues on to the next node in the linked list
	}
}

//Reads the data in the linked list, and prints it in the per-process format
void per_process_print(file_node *head){

    //Initalizes variables needed
    int count = 0;
	file_node *printer_boi = NULL;
	printer_boi = head;

    //Prints the table header columns
    printf("PID       FD\n");
    printf("============\n");

    //Iterates through each node in the linked list and prints all the data in the composite table format
	while(printer_boi != NULL) {                 //Checks if the linked list is null
        if(count != 0){                          //Skip the first node (the head doesn't contain any information)

            //Prints the information contained in the node in the specified format
            printf("%s   %s    \n", printer_boi->PID, printer_boi->FD);
        }
        count++;
		printer_boi = printer_boi->next;
	}
}

//Reads the data in the linked list, and prints it in the systemWide format
void system_wide_print(file_node *head){

    //Initalizes variables needed
    int count = 0;
	file_node *printer_boi = NULL;
	printer_boi = head;
	
    //Prints the table header columns
    printf("PID     FD      Filename\n");
    printf("========================\n");

    //Exactly the same process as the previous printing functions
	while(printer_boi != NULL) {        
        if(count != 0){
            printf("%s   %s    %s\n", printer_boi->PID, printer_boi->FD, printer_boi->Filename);
        }
        count++;
		printer_boi = printer_boi->next;
	}
}

void vnode_print(file_node *head){

    //Initalizes variables needed
    int count = 0;
	file_node *printer_boi = NULL;
	printer_boi = head;
	
    //Prints the table header columns
    printf("PID       Inode\n");
    printf("===============\n");

    //Exactly the same process as the previous printing functions
	while(printer_boi != NULL) {
        if(count != 0){
            printf("%s   %ld    \n", printer_boi->PID, printer_boi->inode_num);
        }
        count++;
		printer_boi = printer_boi->next;
	}
}

//Prints out the threshold table
void threshold_print(file_node *head, unsigned long compared_fd){

    //Initalizes variables needed
    int count = 0;
	file_node *printer_boi = NULL;
	printer_boi = head;
	
    //Prints the table header column
    printf("## Offending processes:\n");

    //Interates through the entire linked list Prints out data if a node has a FD value higher than the one specified
	while(printer_boi != NULL) {
        char *ptr;
        int current_fd = 0;
        current_fd = strtol(printer_boi->FD, &ptr, 10);

        //Prints out data if a node has a FD value higher than the one specified
        if(count != 0 && current_fd > compared_fd){
            printf("%s   (%s)\n", printer_boi->PID, printer_boi->FD);
        }
        count++;
		printer_boi = printer_boi->next;
	}
}

//Reads from a given linked list, and creates a new list with nodes containing the same PID as the one specified
file_node *pick_pid(file_node *head, char *pid){

    //Initalizes variables needed
    int count = 0;
	file_node *selector_boi = NULL;
	selector_boi = head;

    //Initializes the header of a the new linked list
    file_node *new_head = NULL;
    new_head = new_file_node();

	while(selector_boi != NULL) {                                           //Iterating through the list passed
        if(count != 0 && strcmp(selector_boi->PID,pid) == 0){               //Add the node to the new linked list if the PID matches
            insert_at_tail(new_head, selector_boi->PID, selector_boi->inode_num, selector_boi->FD, selector_boi->Filename);
        } 
        count++;
		selector_boi = selector_boi->next;
	}
    return new_head;                                                        //Return points to the head node of the new linked list
}

//Creates a list that has all the file descriptors of all the files on Linux (the ones I have permission to read from)
void insert_data(file_node *list_head){

    //Initalizes variables needed
    DIR *file_pointer;
    DIR *files_link;
    struct dirent *entry; 
    struct dirent *mini_files;
    struct stat file_stat;

    //Opens the /proc file
    file_pointer = opendir("/proc"); 

    //Checks if the file opened isn't null
    if(file_pointer == NULL){
        return;
    }
    while((entry = readdir(file_pointer)) != NULL){                                      //Goes through each subdirectory as they are found
        char *endptr;                                                                    //and saves info in the the dirent struct entry
        int number = strtol(entry->d_name, &endptr, 10);
        
        if (!(*endptr != '\0' || endptr == entry->d_name)) {                             //checks if the PID is an integer
        
            //Concatenates a string to create a file path to be passed into "opendir"
            char path[512] = "/proc/";
            strncat(path, entry->d_name, 256);
            strncat(path, "/fd/", 256);       

            //Open the files found in the subdirectory
            files_link = opendir(path); 

            if(files_link == NULL){
                continue;
            }
            while((mini_files = readdir(files_link)) != NULL){
                char *endptr1;
                number = strtol(mini_files->d_name, &endptr1, 10);  
                
                if (!(*endptr1 != '\0' || endptr == entry->d_name)){                     //checks if the FD is an integer
                    
                    //Concatenates a string to create a file path to be passed into "lstat to fetch file name"
                    char new_path[512];
                    strcpy(new_path, path);
                    strncat(new_path, mini_files->d_name, 256); 

                    lstat(new_path, &file_stat);

                    //Saves the Filename in a string
                    char file_name[STR_LEN];
                    int string_length = readlink(new_path, file_name, 256);
                    file_name[string_length] = '\0';

                    //Inserts all the data found into a file_nodem and add it to the tail of the linked list
                    insert_at_tail(list_head, entry->d_name, file_stat.st_ino, mini_files->d_name, file_name);
                }
            }
            //Closes all the directories that have been opened
            closedir(files_link);
        }
    }
    //Closes all the directories that have been opened
    closedir(file_pointer);
}

void write_in_text(file_node *head){

    //Initalizes variables needed and open the file the function is going to write to
    FILE *file_pointer;
    file_pointer = fopen("compositeTable.txt", "w");
    int count = 0;
	file_node *printer_boi = NULL;
	printer_boi = head;
	
    //Prints the table header column
    fprintf(file_pointer, "PID         INODE       FD    Filename\n");
    fprintf(file_pointer, "======================================\n");

    //Iterates through the entire linked list, and write all of its contents into the text file "compositeTable.txt"
	while(printer_boi != NULL) {
        if(count != 0){
            fprintf(file_pointer, "%s   %lu     %s    %s    \n", printer_boi->PID, printer_boi->inode_num, printer_boi->FD, printer_boi->Filename);
        }
        count++;
		printer_boi = printer_boi->next;
	}
    fclose(file_pointer); 
}

void write_in_binary(file_node *head){

    //Initalizes variables needed and open the file the function is going to write to
    FILE *file_pointer;
    file_pointer = fopen("compositeTable.bin", "wb");
    int count = 0;
	file_node *printer_boi = NULL;
	printer_boi = head;
	
    //Prints the table header column
    char string_title[] = "PID         INODE       FD    Filename\n";
    char string_separator[] = "======================================\n";

    //Writes the header column in binary to the specfied binary file
    fwrite(string_title, sizeof(char), sizeof(string_title), file_pointer);
    fwrite(string_separator, sizeof(char), sizeof(string_separator), file_pointer);

    //Iterates through the entire linked list, and write all of its contents into the binary file "compositeTable.bn"
	while(printer_boi != NULL) {
        if(count != 0){
            fwrite(&printer_boi->PID, sizeof(printer_boi->PID), 1, file_pointer);
            fwrite(&printer_boi->inode_num, sizeof(printer_boi->inode_num), 1, file_pointer);
            fwrite(&printer_boi->FD, sizeof(printer_boi->PID), 1, file_pointer);
            fwrite(&printer_boi->Filename, sizeof(printer_boi->Filename), 1, file_pointer);
        }
        count++;
		printer_boi = printer_boi->next;
	}
    fclose(file_pointer); 
}
