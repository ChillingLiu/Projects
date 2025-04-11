#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memoryBlock {
	size_t start, end;
	char *name;
	struct memoryBlock *prev, *next;
} memoryBlock;

size_t memory_size = 0;
memoryBlock *total_memory;

//Initialization.
int init(int argc, char **argv);
//Free Memory.
void free_memory();

//Checker of request memory.
void request_wrapper();
//Chekcer of releas memory.
void release_wrapper();

//Start requesting memory for process "name".
int request_memory(const char *name, size_t size, char approach);
//For initial total memory allocation and block memory allocation when request memory.
memoryBlock *make_block(size_t start, size_t end, const char *name, memoryBlock *prev, memoryBlock *next);
//Implementation of First-Fit algorithm.
memoryBlock *first_fit(memoryBlock *hole, size_t size);
//Implementation of Best-Fit algorithm.
memoryBlock *best_fit(memoryBlock *hole, size_t size);
//Implementation of Worst-Fit algorithm.
memoryBlock *worst_fit(memoryBlock *hole, size_t size);

//Release all block memory where the "name" used to allocate, and merge unused memory if possible.
int release_memory(const char *name);
//Compact unused holes of memory into one region.
void compact_memory();

//Display the compile usage of this program.
void display_initial_usage();
//Display the usage of this program, how to use it.
void display_usage();
//Display the current status of memory allocation.
void display_memory();



int main(int argc, char **argv) {
	if(init(argc, argv) != 0) {
		display_initial_usage();
		return 0;
	}
	char op[8];
	display_usage();
	while(1) {
		printf("allocator>");
		scanf("%s", op);
		if(strcmp(op, "RQ") == 0) {
			request_wrapper();
		}
		else if(strcmp(op, "RL") == 0) {
			release_wrapper();
		}
		else if(strcmp(op, "C") == 0) {
			compact_memory();
		}
		else if(strcmp(op, "STAT") == 0) {
			display_memory();
		}
		else if(strcmp(op, "help") == 0) {
			display_usage();
		}
		else if(strcmp(op, "X") == 0) {
			free_memory();
			break;
		}
		else {
			printf("Invalid Command! You may use 'help' to check the operations you can do.\n");
		}
	}
	return 0;
}



//Initialization.
int init(int argc, char **argv) {
	if(argc != 2) {
		printf("Incorrect number of arguments.\n");
		return -1;
	}
	sscanf(argv[1], "%zu", &memory_size);
	total_memory = make_block(0, memory_size - 1, "", NULL, NULL);
	printf("The size of total_memory is initialized to %zu bytes\n", memory_size);
	return 0;
}

//Checker of request memory.
void request_wrapper() {
	char name[10], approach;
	size_t size;
	scanf("%s %zu %c", name, &size, &approach); // unsafe but convenient
	printf(request_memory(name, size, approach) ? "FAILED\n" : "SUCCEEDED\n");
}

//Chekcer of releas memory.
void release_wrapper() {
	char name[10];
	scanf("%s", name); // unsafe but convenient
	printf(release_memory(name) ? "FAILED\n" : "SUCCEEDED\n");
}

//Start requesting memory for process "name".
int request_memory(const char *name, size_t size, char approach) {
	memoryBlock *hole = NULL;
	switch(approach) {
		case 'F': {
			hole = first_fit(hole, size);
			break;
		}
		case 'B': {
			hole = best_fit(hole, size);
			break;
		}
		case 'W': {
			hole = worst_fit(hole, size);
			break;
		}
		default: {
			printf("Unknown approach: %c\n", approach);
			return -1;
		}
	}
	if(!hole || hole->name != NULL) {
		printf("No available memory to allocate.\n");
		return -2;
	}
	hole->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(hole->name, name);
	if(hole->end - hole->start + 1 == size) {   //The hole size is exactly equal to the requested size.
		return 0;
	}
	hole->next = make_block(hole->start + size, hole->end, "", hole, hole->next);
	hole->end = hole->start + size - 1;
	return 0;
}

//For initial total memory allocation and block memory allocation when request memory.
memoryBlock *make_block(size_t start, size_t end, const char *name, memoryBlock *prev, memoryBlock *next) {
	memoryBlock *tmp = malloc(sizeof(memoryBlock));
	if(tmp == NULL) {
		printf("Failed to allocate physical memory.\n");
		exit(-1);
	}
	tmp->start = start, tmp->end = end;
	//Allocate memory and copy the string.
	if(strlen(name) != 0) {
		tmp->name = malloc(sizeof(char) * (strlen(name) + 1));
		strcpy(tmp->name, name);
	}
	else { //Unused block.
		tmp->name = NULL;
	}
	//Handle the prev and next to preserve a doubly-linked list.
	tmp->prev = prev, tmp->next = next;
	if(prev) {
		prev->next = tmp;
	}
	if(next) {
		next->prev = tmp;
	}
	return tmp;
}

//Implementation of First-Fit algorithm.
memoryBlock *first_fit(memoryBlock *hole, size_t size) {
	hole = total_memory;
	while(hole) {
		if(hole->name == NULL && (hole->end - hole->start + 1) >= size) {
			break;
		}
		hole = hole->next;
	}
	return hole;
}

//Implementation of Best-Fit algorithm.
memoryBlock *best_fit(memoryBlock *hole, size_t size) {
	memoryBlock *cursor = total_memory;
	size_t min_size = -1;   //Get the max number in size_t.
	while(cursor) {
		size_t hole_size = (cursor-> end - cursor->start + 1);
		if(cursor->name == NULL && size <= hole_size && hole_size < min_size) {
			min_size = hole_size;
			hole = cursor;
		}
		cursor = cursor->next;
	}
	return hole;
}

//Implementation of Worst-Fit algorithm.
memoryBlock *worst_fit(memoryBlock *hole, size_t size) {
	memoryBlock *cursor = total_memory;
	size_t max_size = size - 1;
	while(cursor) {
		size_t hole_size = (cursor-> end - cursor->start + 1);
		if(cursor->name == NULL && hole_size > max_size) {
			max_size = hole_size;
			hole = cursor;
		}
		cursor = cursor->next;
	}
	return hole;
}

//Release all block memory where the "name" used to allocate, and merge unused memory if possible.
int release_memory(const char *name) {
	memoryBlock *cursor = total_memory;
	int flag = 1;
	while(cursor) {
		if(cursor->name && strcmp(cursor->name, name) == 0) {
			free(cursor->name);
			cursor->name = NULL;	//Mark it unused.
			flag = 0;
		}
		//Merge with the prev block if possible.
		if(cursor->name == NULL && cursor->prev && cursor->prev->name == NULL) {
			memoryBlock *temp = cursor->prev;
			cursor->prev = temp->prev;
			if(temp->prev) {
				temp->prev->next = cursor;
			}
			cursor->start = temp->start;
			free(temp);
		}
		//Update the first block in memory if necessary.
		if(cursor->prev == NULL) {
			total_memory = cursor;
		}
		cursor = cursor->next;
	}
	if(flag) {
		printf("No memory gets released!\n");
	}
	return flag;
}

//Compact unused holes of memory into one region.
void compact_memory() {
	memoryBlock *cursor = total_memory;
	while(cursor) {
		//Set unused to used, swap these two blocks
		if(cursor->name && cursor->prev && !cursor->prev->name) {
			memoryBlock *prev = cursor->prev;
			prev->end = prev->start + (cursor->end - cursor->start);
			cursor->start = prev->end + 1;
			prev->name = cursor->name;
			cursor->name = NULL;
		}
		//Set unused to unused, merge thees two blocks
		if(!cursor->name && cursor->prev && !cursor->prev->name) {
			memoryBlock *prev = cursor->prev;
			cursor->start = prev->start;
			cursor->prev = prev->prev;
			if(cursor->prev) {
				cursor->prev->next = cursor;
			}
			free(prev);
		}
		cursor = cursor->next;
	}
}

//Display the compile usage of this program.
void display_initial_usage() {
	printf("You need to pass the initial amount of memory at startup!\n");
	printf("For example: the following initializes the program with 1 MB of memory.\n");
	printf("./allocator 1048576\n\n");
}

//Display the usage of this program, how to use it.
void display_usage() {
	printf("Operations you can do:\n");
	printf("<1>Request memory: RQ <name of the process> <memory size of the process> <approach>\n");
	printf("   (<approach>: F for First-Fit, B for Best-Fit, W for Worst-Fit)\n");
	printf("   For example: 'RQ P1 100 F': use First-Fit to allocate 100 bytes for process P1.\n");
	printf("<2>Release memory: RL <name of the process>\n");
	printf("   For example: 'RL P2': release memory of process P2.\n");
	printf("<3>Compaction: compact unused holes of memory into one region.\n");
	printf("   For example: 'C'\n");
	printf("<4>Display memory: report the regions of memory that are allocated and unused.\n");
	printf("   For example: 'STAT'\n");
	printf("<5>Help: Display this information again.\n");
	printf("   For example: 'help'\n");
	printf("<6>Exit: Exit and close this program.\n");
	printf("   For example: 'X'\n\n");
}

//Display the current status of memory allocation.
void display_memory() {
	memoryBlock *cursor = total_memory;
	while(cursor) {
		printf("Addresses [%06zu : %06zu] ", cursor->start, cursor->end);
		if(cursor->name) {
			printf("Process %s\n", cursor->name);
		}
		else {
			printf("Unused\n");
		}
		cursor = cursor->next;
	}
	printf("\n");
}

//Free Memory.
void free_memory() {
	memoryBlock *temp = total_memory;
	while(total_memory) {
		free(total_memory -> name);
		temp = total_memory;
		total_memory = total_memory -> next;
		free(temp);
	}
}
