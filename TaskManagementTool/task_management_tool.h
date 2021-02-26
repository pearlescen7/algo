/* @Author
Student Name: Uzay Uysal
Student ID : 150180039
Date: 06.11.2019 */
/*
PLEASE DO NOT REMOVE ANY FUNCTION OR VARIABLE IN WORKPLAN STRUCT, IF YOU NEED YOU CAN ADD NEW FUNCTIONS 
*/


#ifndef TASK_MANAGEMENT_TOOL
#define TASK_MANAGEMENT_TOOL

#include <stdio.h>

#define strnicmp strncasecmp

#include "task.h"

struct WorkPlan{
	void create(); //done
	void close(); //done
	void add(Task *task); //done
	void checkAvailableNextTimesFor(Task *delayed); //NOT
	int  getUsableDay(); //done
	int getUsableTime(); //done
	void remove(Task *target); //done
	void display(bool verbose, bool testing); //done
	void delayAllTasksOfDay(int day); //NOT getnexttime check day if not right set and checkagain until can add
	Task * getTask(int day, int time); //done
	bool checkCycledList();

	
	Task *head;
	int usable_day;
	int usable_time;

	int count; //TEST THIS
};
#endif 

