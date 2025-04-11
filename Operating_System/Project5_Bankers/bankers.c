#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Inilization from the textbook.
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

#define INIT_FILE "resources.txt"

int is_leq(int* a, int* b, int n);
//Check if the whether the operation is safe to execute.
int is_safe();
//Implementation of request resources: RQ.
//Return: 0 for success, non-zero for errors.
int request_resources(int customer, int request[NUMBER_OF_RESOURCES]);
//Implemetation of release resources: RL.
//Return: 0 for success, non-zero for errors.
int release_resources(int customer, int release[NUMBER_OF_RESOURCES]);
//Checker of request resources.
void request_wrapper();
//Checker of release resources.
void release_wrapper();
//Display the compile usage of this program.
void display_initial_usage();
//Display the usage of this program, how to use it.
void display_usage();
//Display the information of current resurces.
void display_resources();
//Initialization of resources from the textbook.
int init(int argc, char *argv[], const char *resources_file);

int main(int argc, char *argv[]) {
	if(init(argc, argv, INIT_FILE) != 0) {
		display_initial_usage();
		return 0;
	}
	char op[5];
	display_usage();
	printf(">> ");
	while(scanf("%s", op) == 1) {
		if(strcmp(op, "RQ") == 0) {
			request_wrapper();
		}
		else if(strcmp(op, "RL") == 0) {
			release_wrapper();
		}
		else if(strcmp(op, "*") == 0) {
			display_resources();
		}
		else if(strcmp(op, "help") == 0) {
			display_usage();
		}
		else if(strcmp(op, "exit") == 0) {
			break;
		}
		else {
			printf("Invalid Command! You may use 'help' to check the operations you can do.\n");
		}
		printf(">> ");
	}
	return 0;
}

//Check if the whether the operation is safe to execute.
int is_safe() {
	int work[NUMBER_OF_RESOURCES], finish[NUMBER_OF_CUSTOMERS];
	memcpy(work, available, NUMBER_OF_RESOURCES * sizeof(int));
	memset(finish, 0, NUMBER_OF_CUSTOMERS * sizeof(int));
	for(int round = 0; round != NUMBER_OF_CUSTOMERS; ++round) {
		int flag = 0;
		for(int i = 0; i != NUMBER_OF_CUSTOMERS; ++i) {
			if(finish[i] == 0 && is_leq(need[i], work, NUMBER_OF_RESOURCES)) {
				flag = 1;
				finish[i] = 1;
				for(int j = 0; j != NUMBER_OF_RESOURCES; ++j) {
					work[j] += allocation[i][j];
				}
				break;
			}
		}
		if(!flag) {
			return 0;
		}
	}
	return 1;
}

//Implementation of request resources: RQ.
//Return: 0 for success, non-zero for errors.
int request_resources(int customer, int request[NUMBER_OF_RESOURCES]) {
	if(customer < 0 || customer >= NUMBER_OF_CUSTOMERS) {
		printf("Invalid customer: %d\n", customer);
		return -1;
	}
	int err = 0;
	for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
		if(request[i] < 0 || request[i] > need[customer][i]) {
			printf("Invalid number of resources to request:\n(customer = %d, resource = %d, need = %d, to request = %d)\n",
						customer, i, need[customer][i], request[i]);
			err = -1;
		}
		if(request[i] > available[i]) {
			printf("No enough resources to allocate:\n(customer = %d, resource = %d, available = %d, to request = %d)\n",
						customer, i, available[i], request[i]);
			err = -2;
		}
		if(err != 0) {//rollback
			while(i--) {
				available[i] += request[i];
				allocation[customer][i] -= request[i];
				need[customer][i] += request[i];
			}
			return err;
		}
		// allocate resources
		available[i] -= request[i];
		allocation[customer][i] += request[i];
		need[customer][i] -= request[i];
	}
	if(!is_safe()) {
		// rollback
		printf("Unsafe state after request!\n");
		for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
			available[i] += request[i];
			allocation[customer][i] -= request[i];
			need[customer][i] += request[i];
		}
		return -3;
	}
	return 0;
}


//Implemetation of release resources: RL.
//Return: 0 for success, non-zero for errors.
int release_resources(int customer, int release[NUMBER_OF_RESOURCES]) {
	if(customer < 0 || customer >= NUMBER_OF_CUSTOMERS) {
		printf("Invalid customer: %d\n", customer);
		return -1;
	}
	for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
		if(release[i] < 0 || release[i] > allocation[customer][i]) {
			printf("Invalid number of resources to release:\n(customer = %d, resource = %d, allocated = %d, to release = %d)\n",
					customer, i, allocation[customer][i], release[i]);
			// rollback
			while(i--) {
				allocation[customer][i - 1] += release[i - 1];
				available[i] -= release[i];
			}
			return -1;
		}
		// release resources
		allocation[customer][i] -= release[i];
		available[i] += release[i];
	}
	return 0;
}

//Checker of request resources.
void request_wrapper() {
	int request[NUMBER_OF_RESOURCES], customer;
	scanf("%d", &customer);
	for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
		scanf("%d", &request[i]);
	}
	if(request_resources(customer, request) != 0) {
		printf("FAILED.\n");
	}
	else {
		printf("SUCCEEDED.\n");
	}
}

//Checker of release resources.
void release_wrapper() {
	int release[NUMBER_OF_RESOURCES], customer;
	scanf("%d", &customer);
	for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
		scanf("%d", &release[i]);
	}
	if(release_resources(customer, release) != 0) {
		printf("FAILED.\n");
	}
	else {
		printf("SUCCEEDED.\n");
	}
}

//Display the compile usage of this program.
void display_initial_usage() {
	printf("The number of resourses type is %d.\n", NUMBER_OF_RESOURCES);
	printf("You need to enter the number of instances of each type.\n");
	printf("For example: './bankers 10 6 7 8' for number of type = 4.\n");
}

//Display the usage of this program, how to use it.
void display_usage() {
	printf("Operations you can do:\n");
	printf("<1>Request resources: RQ <index of customer> <request resources of each type>\n");
	printf("   For example: '>> RQ 0 3 1 2 1': the 0th customer requests resources 3 1 2 1 respectively.\n");
	printf("<2>Release resources: RL <index of customer> <request resources of each type>\n");
	printf("   For example: '>> RL 0 1 1 1 1': the 0th customer release resources 1 1 1 1 respectively.\n");
	printf("<3>Display resources: '*'\n");
	printf("   It will show you the current informations (Availible, Maximum, Allocated, Needed) of each customer.\n");
	printf("<4>Help: 'help'\n");
	printf("   It will show you this information (usage) again.\n");
	printf("<5>Exit: 'exit'\n");
	printf("   Close and exit the program.\n\n");
}

//Display the information of current resurces.
void display_resources() {
	printf("Availbale resources:\n");
	for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
		printf("%d ", available[i]);
	}
	printf("\n");
	printf("Maximum resources for each customer:\n");
	for(int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) {
		printf("%d: ", customer);
		for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
			printf("%d ", maximum[customer][r]);
		}
		printf("\n");
	}
	printf("\n");
	printf("Allocated resources for each customer:\n");
	for(int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) {
		printf("%d: ", customer);
		for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
			printf("%d ", allocation[customer][r]);
		}
		printf("\n");
	}
	printf("\n");
	printf("Needed resources for each customer:\n");
	for(int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) {
		printf("%d: ", customer);
		for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
			printf("%d ", need[customer][r]);
		}
		printf("\n");
	}
}

//Initialization of resources from the textbook.
int init(int argc, char *argv[], const char *resources_file) {
	if(argc != 1 + NUMBER_OF_RESOURCES) {
		printf("Incorrect number of parameters.\n");
		return -1;
	}
	for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
		available[i] = atoi(argv[i + 1]);
	}
	FILE *f = fopen(resources_file, "r");
	if(f == NULL) {
		printf("Unable to open file: %s\n", resources_file);
		printf("The resoureces.txt is the initialization from the textbook\n");
		return -2;
	}
	for(int c = 0; c != NUMBER_OF_CUSTOMERS; ++c) {
		for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
			fscanf(f, "%d", &maximum[c][r]);
			need[c][r] = maximum[c][r];
		}
	}
	fclose(f);
	return 0;
}

int is_leq(int* a, int* b, int n) {
	for(int i = 0 ; i != n; ++i) {
		if(a[i] > b[i]) {
			return 0;
		}
	}
	return 1;
}
