#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

#define STRING_NUMBER 20
int page_faults;

//Generates a random page-reference string.
int* random_string();
//Initialization of frames before FIFO, LRU, and Optimal.
void initial_frames(int* frames, int frames_number);
//Checks if the current data exists in the frames for FIFO, LRU, and Optimal.
bool exists(int data, int* frames, int frames_number);
//In order to show the current status of frames.
void print_array(int* array, int length);
//Implements First In First Out (FIFO) page-replacement algorithm
int fifo(int* reference_string, int* frames, int frames_number);
//Implements Least Recently Used (LRU) page-replacement algorithm
int lru(int* reference_string, int* frames, int frames_number);
//Implements Optimal page-replacement algorithm
int optimal(int* reference_string, int* frames, int frames_number);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Error: You must provide the number of page frames as an argument.\n");
		exit(EXIT_FAILURE);
	}
	int frames_number = atoi(argv[1]);
	int* frames = (int*)malloc(frames_number * sizeof(int));

	int* reference_string = random_string();
	printf("Random Reference String:\n");
	print_array(reference_string, STRING_NUMBER);
	printf("\n");

	printf("Start Alogorithm of FIFO:\nStatus of frames:\n");
	printf("Page faults of FIFO: %d\n\n", fifo(reference_string, frames, frames_number));

	printf("Start Alogorithm of LRU:\nStatus of frames:\n");
	printf("Page faults of LRU: %d\n\n", lru(reference_string, frames, frames_number));

	printf("Start Alogorithm of Optimal:\nStatus of frames:\n");
	printf("Page faults of Optimal: %d\n\n", optimal(reference_string, frames, frames_number));

	free(frames);
	free(reference_string);
	return 0;
}



//Generates a random page-reference string.
int* random_string() {
	int* string = (int*)malloc(STRING_NUMBER * sizeof(int));
	srand(time(NULL));
	for (int i = 0; i < STRING_NUMBER; i++) {
		string[i] = rand() % 10;
	}
	return string;
}

//Initialization of frames before FIFO, LRU, and Optimal.
void initial_frames(int* frames, int frames_number) {
	for (int i = 0; i < frames_number; i++) {
		frames[i] = -1;
	}
}

//Checks if the current data exists in the frames for FIFO, LRU, and Optimal.
bool exists(int data, int* frames, int frames_number) {
	int k = 0;
	while (k < frames_number) {
		if (frames[k] == data) {
			return true;
		}
		k++;
	}
	return false;
}

//In order to show the current status of frames.
void print_array(int* array, int length) {
	for (int i = 0; i < length; i++) {
		if(array[i] == -1) {
			printf("  ");
		}
		else {
			printf("%d ", array[i]);
		}
	}
	printf("\n");
}

//Implements First In First Out (FIFO) page-replacement algorithm
int fifo(int* reference_string, int* frames, int frames_number) {
	page_faults = 0;
	initial_frames(frames, frames_number);

	int i, j = 0;
	for (i = 0; i < STRING_NUMBER; i++) {
		if (!exists(reference_string[i], frames, frames_number)) {
			for (j = 0; j < frames_number - 1; j++) {
				frames[j] = frames[j + 1];
			}
			frames[j] = reference_string[i];
			page_faults++;
			print_array(frames, frames_number);
		}
	}
	return page_faults;
}

//Implements Least Recently Used (LRU) page-replacement algorithm
int lru(int* reference_string, int* frames, int frames_number) {
	page_faults = 0;
	initial_frames(frames, frames_number);
	int* close = (int*)malloc(frames_number * sizeof(int));

	int i, j, k = 0;
	while (k < STRING_NUMBER) {
		if (!exists(reference_string[k], frames, frames_number)) {
			for (i = 0; i < frames_number; i++) {
				int find = 0;
				int frame = frames[i];
				j = k - 1;
				while (j >= 0) {
					if (frame == reference_string[j]) {
						find = 1;
						close[i] = j;
						break;
					}
					else {
						find = 0;
					}
					j--;
				}
				if (!find) {
					close[i] = -99;
				}
			}
			int least = 99;
			int repeated;
			i = 0;
			while (i < frames_number) {
				if (close[i] < least) {
					repeated = i;
					least = close[i];
				}
				i++;
			}
			frames[repeated] = reference_string[k];
			page_faults++;
			print_array(frames, frames_number);
		}
	k++;
	}
	free(close);
	return page_faults;
}

//Implements Optimal page-replacement algorithm
int optimal(int* reference_string, int* frames, int frames_number) {
	page_faults = 0;
	initial_frames(frames, frames_number);
	int* close = (int*)malloc(frames_number * sizeof(int));

	int i, j, k = 0;
	while (k < STRING_NUMBER) {
		if (!exists(reference_string[k], frames, frames_number)) {
			for (i = 0; i < frames_number; i++) {
				int find = 0;
				int frame = frames[i];
				j = k;
				while (j < STRING_NUMBER) {
					if (frame == reference_string[j]) {
						find = 1;
						close[i] = j;
						break;
					}
					else {
						find = 0;
					}
					j++;
				}
				if (!find) {
					close[i] = 99;
				}
			}
			int maximum = -99;
			int repeated;
			i = 0;
			while (i < frames_number) {
				if (maximum < close[i]) {
					repeated = i;
					maximum = close[i];
				}
				i++;
			}
			frames[repeated] = reference_string[k];
			page_faults++;
			print_array(frames, frames_number);
		}
		k++;
	}
	free(close);
	return page_faults;
}
