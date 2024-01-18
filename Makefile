
#defining variables
DEPENDENCIES = A2_linked_list.h A2_functions.h

#makes the entire project
all: driver.o A2_functions.o A2_linked_list.o
	gcc driver.o A2_functions.o A2_linked_list.o

#compile the main project object file
driver.o: driver.c ${DEPENDENCIES}
	gcc -c driver.c

#compiles the functions and linked list files.c files into object files
%.o: %.c ${DEPENDENCIES}
	gcc -c $<

#Removes all object files
.PHONY: clean 

clean:
	-rm *.o
