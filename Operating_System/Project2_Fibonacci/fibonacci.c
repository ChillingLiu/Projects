#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int i; //counter for thread
unsigned long long* fibonacci_sequence; //shared memory

void *fibonacci_thread(void* params);
void user_input(int* numbers);

int main() {
	int numbers = 0; //numbers of Fibonacci sequence
	user_input(&numbers); // get user input

	fibonacci_sequence = (unsigned long long*)malloc(numbers * sizeof(unsigned long long));

	pthread_t *tid = (pthread_t*)malloc(numbers * sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	//do "numbers" times thread, every thread gives one number of Fibonacci
	for(i = 0; i < numbers; i++) {
		pthread_create(&tid[i], &attr, fibonacci_thread, NULL); //start
		pthread_join(tid[i], NULL); //wait
	}

	//output
	for(int j = 0; j < numbers; j++) {
		printf("%llu ", fibonacci_sequence[j]);
	}
	printf("\nDone!\n");

	free(fibonacci_sequence);
	free(tid);
	return 0;
 }

void *fibonacci_thread(void* params) {
	if(i == 0) {
		fibonacci_sequence[i] = 0;
		pthread_exit(0);
	}

	else if(i == 1) {
		fibonacci_sequence[i] = 1;
		pthread_exit(0);
	}

	else {
		fibonacci_sequence[i] = fibonacci_sequence[i-1] + fibonacci_sequence[i-2];
		pthread_exit(0);
	}
}

void user_input(int* numbers) {
	printf("Enter a number of Fibonacci numbers you want to generate the sequence: \n");
	scanf("%d", numbers);
}
