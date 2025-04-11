#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define OFFSET_BITS 8
#define PAGE_BITS 8
#define FRAME_BITS 7	//The physical memory is smaller than the virtual one.

//"<<" is Left Shift for binary number. In decimal: a = a << b => a = a * (2^b).
//Foe example: 1 << 2 => 00000000 00000001 => 00000000 000000100, 1 = 1 * 2^2.

//Since the logical addresses in addresses.txt are all decimal values,
//we need to convert them to binary value first,
//then the indexes 0 - 7 is the offset, and indexes 8 - 15 is page number.
//The maximal logical address is 2^16 = 65536,
//so we only need to consider the first 16 bits in binary representation.
#define PAGE_SIZE (1 << OFFSET_BITS)
#define FRAME_SIZE PAGE_SIZE
#define TLB_SIZE 16
#define NUMBER_OF_PAGES (1 << PAGE_BITS)
#define NUMBER_OF_FRAMES (1 << FRAME_BITS)
#define MEMORY_SIZE (FRAME_SIZE * NUMBER_OF_FRAMES)

//Macros functions.
#define GET_PAGE_NUMBER(address) ((address & ((1 << (OFFSET_BITS + PAGE_BITS))- (1 << OFFSET_BITS))) >> OFFSET_BITS)
#define GET_OFFSET(address) (address & ((1 << OFFSET_BITS) - 1))
#define GET_PHYSICAL_ADDRESS(frame, offset) ((frame_number << OFFSET_BITS) | offset)

#define BACKING_STORAGE_FILE "BACKING_STORE.bin"

typedef struct {
	uint8_t valid;
	uint32_t page, frame;
} Pair;
Pair TLB[TLB_SIZE];

//Global Variables.
int8_t memory[MEMORY_SIZE];
uint32_t page_table[NUMBER_OF_PAGES], next_available_frame, next_available_TLB;
uint8_t page_valid[NUMBER_OF_PAGES];
FILE *backing_storage, *input_file;
//For calculating Page-Fault rate and TLB-Hit rate.
uint32_t count_total, count_page_fault, count_TLB_miss;

//Initialization.
int init(int argc, char **argv);

//Translate logical address to its corresponding physical address.
uint32_t translate_address(uint32_t logical);
//Check whether the current page_number is inside the TLB.
int check_TLB(uint32_t page_number, uint32_t *frame_number);
//We need to update the current TLB once we get a TLB-miss.
void update_TLB(uint32_t page_number, uint32_t frame_number);
//When a Page-Fault occurs, we will read in a 256-byte page from the file
//BACKING_STORE and store it in an available page frame in physical memory.
void handle_page_fault(uint32_t page_number);
//Part of Handle Page-Fault.
uint32_t select_victim_frame();

//Get the value of the byte stored at the translated physical address.
char access_memory(uint32_t physical);
//Display the compile usage of this program.
void display_initial_usage();
//Display the Page-Fault rate and TLB-Hit rate.
void display_statistics();
//Free Memory.
void free_memory();



int main(int argc, char **argv) {
	if(init(argc, argv) != 0) {
		display_initial_usage();
		free_memory();
		return 0;
	}
	//Read logical addresses from input file.
	char line[8];
	while(fgets(line, 8, input_file)) {
		uint32_t logical, physical;
		int8_t value;
		sscanf(line, "%u", &logical);
		physical = translate_address(logical);
		//Get the value of the byte stored at the translated physical address.
		value = access_memory(physical);
		printf("Virtual address: %u Physical address: %u Value: %d\n", logical, physical, value);
	}
	display_statistics();
	free_memory();
	return 0;
}

//Initialization.
int init(int argc, char **argv) {
	if(argc != 2) {
		printf("Incorrect number of arguments.\n");
		return -1;
	}
	//"rb" to read binary file.
	backing_storage = fopen(BACKING_STORAGE_FILE, "rb");
	if(backing_storage == NULL) {
		printf("Unable to open the backing storage file: %s\n", BACKING_STORAGE_FILE);
		return -2;
	}
	input_file = fopen(argv[1], "r");
	if(input_file == NULL) {
		printf("Unable to open the input file: %s\n", argv[1]);
		return -3;
	}
	//At first, both the memory and page table are empty.
	memset(page_valid, 0, sizeof(uint8_t) * NUMBER_OF_PAGES);
	next_available_frame = next_available_TLB = 0;
	return 0;
}

//Translate logical address to its corresponding physical address.
uint32_t translate_address(uint32_t logical) {
	++count_total;
	uint32_t page_number, offset, frame_number;
	//Macros functions.
	page_number = GET_PAGE_NUMBER(logical);
	offset = GET_OFFSET(logical);

	if(!check_TLB(page_number, &frame_number)) {
		//TLB-miss
		++count_TLB_miss;
		if(page_valid[page_number] == 0) {
			//Page-Fault.
			handle_page_fault(page_number);
		}
		frame_number = page_table[page_number];
		//We need to update the current TLB once we get a TLB-miss.
		update_TLB(page_number, frame_number);
	}
	//Macros Function, give the physical address.
	return GET_PHYSICAL_ADDRESS(frame_number, offset);
}

//Check whether the current page_number is inside the TLB.
int check_TLB(uint32_t page_number, uint32_t *frame_number) {
	for(size_t i = 0; i != TLB_SIZE; ++i) {
		//If it's inside, the frame number is obtained directly.
		if(TLB[i].valid && TLB[i].page == page_number) {
			*frame_number = TLB[i].frame;
			return 1;
		}
	}
	return 0;
}

//We need to update the current TLB once we get a TLB-miss.
void update_TLB(uint32_t page_number, uint32_t frame_number) {
	//FIFO.
	size_t victim = next_available_TLB % TLB_SIZE;
	next_available_TLB = (next_available_TLB + 1) % TLB_SIZE;
	TLB[victim].valid = 1;
	TLB[victim].page = page_number, TLB[victim].frame = frame_number;
}

//When a Page-Fault occurs, we will read in a 256-byte page from the file
//BACKING_STORE and store it in an available page frame in physical memory.
void handle_page_fault(uint32_t page_number) {
	page_table[page_number] = select_victim_frame();
	fseek(backing_storage, page_number * PAGE_SIZE, SEEK_SET);
	fread(memory + page_table[page_number] * PAGE_SIZE, sizeof(int8_t), PAGE_SIZE, backing_storage);
	page_valid[page_number] = 1;
	++count_page_fault;
}

//Part of Handle Page-Fault.
uint32_t select_victim_frame() {
	//FIFO.
	if(next_available_frame < NUMBER_OF_FRAMES) {
		return next_available_frame++;
	}
	uint32_t victim = (next_available_frame++) % NUMBER_OF_FRAMES;
	for(size_t i = 0; i != NUMBER_OF_PAGES; ++i) {  //Invalidate the victim page.
		if(page_valid[i] && page_table[i] == victim) {
			page_valid[i] = 0;
			break;
		}
	}
	return victim;
}

//Get the value of the byte stored at the translated physical address.
char access_memory(uint32_t physical) {
	return memory[physical];
}

//Display the compile usage of this program.
void display_initial_usage() {
	printf("You need to pass the file of logical addresses!\n");
	printf("./manager <input file>, For example: ./manager addresses.txt\n\n");
}

//Display the Page-Fault rate and TLB-Hit rate.
void display_statistics() {
	printf("\nTotal: %d, Page-Fault: %d, TLB-Hit: %d\n", count_total, count_page_fault, count_total - count_TLB_miss);
	printf("Page-Fault rate = %.2f%%\n", (float)count_page_fault / count_total * 100);
	printf("TLB-Hit rate = %.2f%%\n", (float)(count_total - count_TLB_miss) / count_total * 100);
}

//Free Memory.
void free_memory() {
	if(input_file) {
		fclose(input_file);
	}
	if(backing_storage) {
		fclose(backing_storage);
	}
}

