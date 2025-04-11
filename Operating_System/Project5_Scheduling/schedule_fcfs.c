#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

//Structure from "list.h".
struct node *taskList = NULL;

//Implementation of the function "add()" in schedulers.h.
//Add the current task to the taskList.
void add(char *name, int priority, int burst) {
	//Struture from "task.h".
	Task* task = (Task*)malloc(sizeof(Task));

	//Allocate the memory of the name (char*) first.
	task->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(task->name, name);
	task->priority = priority;
	task->burst = burst;

	//Function from "list.h".
	insert(&taskList, task);
}

//Pick the next task to execute with FCFS.
Task* pickNextTask() {
	struct node *lastNode = taskList;
	while(1) {
		if(!lastNode->next) {
			break;
		}
		lastNode = lastNode->next;
	}
	return lastNode->task;
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

	while(taskList) {
		Task *task = pickNextTask();
		run(task, task->burst);

		//Calculating:
		timeCounter += task->burst;
		processCounter++;
		task->turnaround = timeCounter;
		task->waiting = task->turnaround - task->burst;
		task->response = timeCounter;

		turnaroundCounter += task->turnaround;
		waitingCounter += task->waiting;
		responseCounter += task->response;
		delete(&taskList, task);
	}
	printf("Average Turnaround Time = %d\n", turnaroundCounter / processCounter);
	printf("Average Waiting Time = %d\n", waitingCounter / processCounter);
	printf("Average Response Time = %d\n", responseCounter / processCounter);
}
