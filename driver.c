#include <sys/sysinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <utmp.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

#include "A2_functions.h"
#include "A2_linked_list.h"

int main(int argc, char **argv){
    int c; 
    int per_process = 0;
    int systemWide = 0;
    int Vnodes = 0;
    int composite = 0;
    int threshold = 0;
    int output_TXT = 0;
    int output_binary = 0;
    bool positional = false;

    //Checks for positional arguments (PID)
    if(argc > 1 && argv[1][0] != '-'){
        positional = true;
    }
    
    //Standard behavior for no flagged arguments
    if(argc == 1) {
        //Creates a head node for a linked list
        file_node *list_head = NULL;
        list_head = new_file_node();

        //Inserts the head and creates a list by fetching all the data and adding them to the end of the list
        insert_data(list_head);
        //Prints the data of each node, starting from the head
        composite_print(list_head);

        //Delete linked list and free all allocated memory
        delete_entire_list(list_head);
    } else if(argc == 2 && positional == true){
        //Creates a composite table with only the files with the specified PID
        composite = 1;
    }

    //Parses the flags specified by the user
    while(true){
        int option_index = 0;
        static struct option long_options[] = {
                   {"per-process", no_argument, NULL, 0},
                   {"systemWide",  no_argument, NULL, 0},
                   {"Vnodes", no_argument, NULL, 0},
                   {"composite", no_argument, NULL, 0},
                   {"threshold", required_argument, NULL, 0},
                    {"output_TXT", no_argument, NULL, 0},
                   {"output_binary", no_argument, NULL, 0},
                };

        c = getopt_long(argc, argv, "",long_options, &option_index);
        
        if (c == -1){break;}

        switch (option_index) {
            case 0:                          
                per_process = 1;                
                break;
            case 1:                             
                systemWide = 1;
                break;
            case 2:             
                Vnodes = 1;
                break;
            case 3:      
                composite = 1;
                break;
            case 4:
                threshold = strtol(optarg, NULL, 10);
                break;
            case 5:
                output_TXT = 1;
                break;
            case 6:
                output_binary = 1;
                break;
            
            default:
                printf("this is not supposed to happen");
            }
        }
        
        //Calls the necessary functions to create and print the composite table 
        if(composite == 1){

            //Behavior if the user specified to filter by a specific UID
            if(positional == true){
                //Creates a head node for a linked list
                file_node *list_head = NULL;
                list_head = new_file_node();

                //Inserts the head and creates a list by fetching all the data and adding them to the end of the list
                insert_data(list_head);

                //Creates a head node for a new linked list, with only the information of the files with the specified PID
                file_node *new_head = NULL;
                new_head = new_file_node();

                //Creates and new linked list by adding nodes from the old list only if the PID matches the one specfied by the user
                new_head = pick_pid(list_head, argv[argc-1]);

                //Deletes and frees the original linked list
                delete_entire_list(list_head);

                //Prints contents of the new linked list in the specified table
                composite_print(new_head);

                //Deletes and frees the new filtered linked list
                delete_entire_list(new_head);
            } 
            
            //Behavior if the user doesn't filter by a specific UID
            else{
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);
                composite_print(list_head);
                delete_entire_list(list_head);
            }
        }

        /*The processes below are very similar to the other to the one you've seen above. Only the printing format is different,
        so we call a different function, so I won't be documenting the parts where they are the same to avoid redundancy.*/

        //Calls the necessary functions to create and print the per-process table
        if(per_process == 1){
            if(positional == true){
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);
                file_node *new_head = NULL;
                new_head = new_file_node();
                new_head = pick_pid(list_head, argv[argc-1]);
                delete_entire_list(list_head);

                //Prints out the data in the per-process format, showing only PID and FD
                per_process_print(new_head);
                delete_entire_list(new_head);
            } else {
                 file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);

                //Prints out the data in the per-process format, showing only PID and FD
                per_process_print(list_head);
                delete_entire_list(list_head);
            }
        }

        //Calls the necessary functions to create and print the systemWide table 
        if(systemWide == 1){
            if(positional == true){
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);
                file_node *new_head = NULL;
                new_head = new_file_node();
                new_head = pick_pid(list_head, argv[argc-1]);
                delete_entire_list(list_head);

                //Prints out the data in the systemWide format, showing only PID, FD, and Filename
                system_wide_print(new_head);
                delete_entire_list(new_head);
            } else{
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);

                //Prints out the data in the systemWide format, showing only PID, FD, and Filename
                system_wide_print(list_head);
                delete_entire_list(list_head);
            }
        }

        //Calls the necessary functions to create and print the Vnodes table 
        if(Vnodes == 1){
            if(positional == true){
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);
                file_node *new_head = NULL;
                new_head = new_file_node();
                new_head = pick_pid(list_head, argv[argc-1]);
                delete_entire_list(list_head);

                //Prints out the data in the systemWide format, showing only PID and Inode number
                vnode_print(new_head);
                delete_entire_list(new_head);
            } else {
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);

                //Prints out the data in the systemWide format, showing only PID and Inode number
                vnode_print(list_head);
                delete_entire_list(list_head);
            }
        }

        //Calls the necessary functions to create and print the threshold table at the end  
        if(threshold != 0){
            if(positional == true){
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);
                file_node *new_head = NULL;
                new_head = new_file_node();
                new_head = pick_pid(list_head, argv[argc-1]);
                delete_entire_list(list_head);

                //Prints all the files with a FD that is higher than the ones specified, prints at the very end of the program
                threshold_print(new_head, threshold);
                delete_entire_list(new_head);
            } else {
                file_node *list_head = NULL;
                list_head = new_file_node();
                insert_data(list_head);

                //Prints all the files with a FD that is higher than the ones specified, prints at the very end of the program
                threshold_print(list_head, threshold);
                delete_entire_list(list_head);
            }
        }

        //BONUS FUNCTION: Write to the contents of the composite table to the "compositeTable.txt" file
        if(output_TXT  == 1){

            //Creates a linked list including data of all the files using the same process as above
            file_node *list_head = NULL;
            list_head = new_file_node();
            insert_data(list_head);

            //Writes all the data of the linked list in a text file
            write_in_text(list_head);

            //Deletes and frees the linked list
            delete_entire_list(list_head);
        }

        //BONUS FUNCTION: Write to the contents (in binary) of the composite table to the "compositeTable.bn" file
        if(output_binary == 1){

            //Creates a linked list including data of all the files using the same process as above
            file_node *list_head = NULL;
            list_head = new_file_node();
            insert_data(list_head);

            //Writes all the data of the linked list in a binary file
            write_in_binary(list_head);

            //Deletes and frees the linked list
            delete_entire_list(list_head);
        }
    return 0;
}