#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

//This program Priority onlys changes some part of the function
//"pickNextTask()" from program SJF.

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

//Pick the next task (highest priority in the queue) to execute.
Task* pickNextTask() {
	Task* highest_priority_task = taskList->task;
	struct node* Node = taskList;

	//Find the task has the highest priority.
	while(Node) {
		if(Node->task->priority >= highest_priority_task->priority) {
			highest_priority_task = Node->task;
		}
		Node = Node->next;
	}
	return highest_priority_task;
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
