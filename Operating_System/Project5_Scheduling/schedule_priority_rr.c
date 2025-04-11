#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

//This program RR changes some part of the function
//"add()", "pickNextTask()" and "schedule()" from program Priority.

//In order to perform the RR algorithm, we have to consider its
//remaining burst everytime, so I add an element "int remaining_burst"
//in the struct of Task from task.h.

//Structure from "list.h".
//MAX_PRIORITY = 10.
struct node *taskList[MAX_PRIORITY + 1];
struct node *next_node;

//Implementation of the function "add()" in schedulers.h.
//Add the current task to the taskList.
void add(char *name, int priority, int burst) {
	//Struture from "task.h".
	Task* task = (Task*)malloc(sizeof(Task));
	//Allocate the memory of the name (char*) first.
	task->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(task->name, name);
	task->priority = priority;
	task->burst = task->remaining_burst = burst;

	//In order to get the response time of each process.
	task->response = 0;
	//Function from "list.h".
	insert(&taskList[priority], task);
}

//Pick the next task to execute with RR.
//We can not use the global varaible taskList now,
//because taskList is a pointer that has MAX_PRIORITY+1 elements.
Task* pickNextTask(struct node* tl) {
	Task* temp = next_node->task;
	if (next_node->next) {
		next_node = next_node->next;
	}
	else {
		next_node = tl;
	}
	return temp;
}

//Implementation of the function "schedule()" in schedulers.h.
void schedule() {
	//Used for calculating the average of:
	//Turnaround Time = Exit Time - Arrival Time,
	//Waiting Time = Turnaround Time - Burst Time,
	//Response Time = Time when the process gets CPU - Arrival Time.
	int timeCounter = 0;
	int processCounter = 0;
	int turnaroundCounter = 0;
	int waitingCounter = 0;
	int responseCounter = 0;

	//From higher priority to lower priority.
	for (size_t i = MAX_PRIORITY; i >= MIN_PRIORITY; i--) {
		next_node = taskList[i];
		while(taskList[i]) {
			Task *task = pickNextTask(taskList[i]);

			//Calculating Response Time.
			if (task->response == 0) {
				task->response = timeCounter;
				responseCounter += task->response;
			}

			//Quantum is 10 milliseconds.
			int slice;
			if (QUANTUM < task->remaining_burst) {
				slice = QUANTUM;
			}
			else {
				slice = task->remaining_burst;
			}

			run(task, slice);
			task->remaining_burst -= slice;
			timeCounter += slice;

			//Once a process is finished, we will record
			//its turnaround and waiting time.
			if (!task->remaining_burst) {
				processCounter++;
				task->turnaround = timeCounter;
				task->waiting = task->turnaround - task->burst;

				turnaroundCounter += task->turnaround;
				waitingCounter += task->waiting;
				delete(&taskList[i], task);
			}
		}
	}
	printf("Average Turnaround Time = %d\n", turnaroundCounter / processCounter);
	printf("Average Waiting Time = %d\n", waitingCounter / processCounter);
	printf("Average Response Time = %d\n", responseCounter / processCounter);
}
