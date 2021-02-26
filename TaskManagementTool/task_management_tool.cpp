/* @Author
Student Name: Uzay Uysal
Student ID : 150180039
Date: 06.11.2019 */
/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}

void WorkPlan::create()
{	
	this->head = NULL;
	this->usable_day = 0;
	this->usable_time = 0;
	this->count = 0;
}

void WorkPlan::close()
{
	Task *traverse = new Task;
	Task *tDeleter = new Task;
	Task *tctr = new Task;
	Task *tctrDeleter = new Task;
	traverse = this->head;
	while(this->count > 0)
	{
		tctr = traverse->counterpart;
		while(tctr != NULL){
			tctrDeleter = tctr;
			tctr = tctr->counterpart;
			this->remove(tctrDeleter);
		}
		tDeleter = traverse;
		traverse = traverse->next;
		this->head = this->head->next;
		this->remove(tDeleter);
	}
	this->head = NULL;
	usable_time = 0;
	usable_day = 0;
}

void WorkPlan::add(Task *task)
{
	//create a new task
	Task *newTask = new Task();
	char* newName = new char [strlen(task->name)+1];
	strcpy(newName, task->name);
	newName[strlen(task->name)] = '\0';
	newTask->name = newName;
	newTask->day = task->day;
	newTask->time = task->time;
	newTask->priority = task->priority;
	newTask->previous = NULL;
	newTask->next = NULL;
	newTask->counterpart = NULL;
	bool added = false;
	//add the first task
	if(this->count == 0)
	{
		this->head = newTask;
		this->head->next = newTask;
		this->head->previous = newTask;
		this->count++;
		added = true;
	} else
	{
		Task *traverse = new Task;
		traverse = this->head;
		//do once since traverse is head in the first iteration
		do
		{
			//if the day doesn't exist insert it
			if(newTask->day < traverse->day)
			{
				newTask->previous = traverse->previous;
				newTask->next = traverse;
				traverse->previous = newTask;
				newTask->previous->next = newTask;
				//move head if the insertion occurs in the first iteration
				if(traverse == this->head){
					this->head = newTask;
				}
				this->count++;
				added = true;
				break;
			}
			//if it does check for the other conditions
			else if(newTask->day == traverse->day)
			{	
				//iterate over counterparts
				Task *traverseCounterpart = new Task;
				traverseCounterpart = traverse;
				Task *traverseInFront = new Task;
				traverseInFront = traverse;
				Task *traverseCtrBehind = new Task;
				while(traverseCounterpart != NULL)
				{
					traverseInFront = traverseInFront->counterpart;
					if(newTask->time == traverseCounterpart->time)
					{
						//if they have the same time check priority
						if(newTask->priority > traverseCounterpart->priority)
						{
							//delay traverseCounterpart
							if(traverseCounterpart->next != NULL)
							{
								if(this->head == traverseCounterpart)
								{
									this->head = newTask;
								}
								newTask->previous = traverseCounterpart->previous;
								newTask->next = traverseCounterpart->next;
								newTask->previous->next = newTask;
								newTask->next->previous = newTask;
								newTask->counterpart = traverseCounterpart->counterpart;
								//this->count++;
							} else{ //found the task before tctr and inserted newTask
								traverseCtrBehind = traverse;
								while(traverseCtrBehind->counterpart != traverseCounterpart){
									traverseCtrBehind = traverseCtrBehind->counterpart;
								}
								traverseCtrBehind->counterpart = newTask;
								newTask->counterpart = traverseCounterpart->counterpart;
							}
							
							this->checkAvailableNextTimesFor(traverseCounterpart);
							traverseCounterpart->time = usable_time;
							traverseCounterpart->day = usable_day;
							traverseCounterpart->next = NULL;
							traverseCounterpart->previous = NULL;
							//insert tctrprt in the right place if in the same day after the delay
							bool dayFound = false;
							bool setToDay = false;
							Task *findDay = new Task;
							findDay = newTask;
							do
							{
								if(findDay->day == traverseCounterpart->day)
								{
									Task *temp = new Task;
									Task *tempBehind = new Task;
									bool firstCheck = false;
									temp = findDay;
									tempBehind = findDay;
									while(temp->counterpart != NULL)
									{
										
										if((!firstCheck) && (traverseCounterpart->time < temp->time)){
											traverseCounterpart->next = temp->next;
											traverseCounterpart->previous = temp->previous;
											temp->previous->next = traverseCounterpart;
											temp->next->previous = traverseCounterpart;
											traverseCounterpart->counterpart = temp;
											temp->next = NULL;
											temp->previous = NULL;
											break;
										}
										firstCheck = true;
										temp = temp->counterpart;
										if(traverseCounterpart->time < temp->time)
										{
											tempBehind->counterpart = traverseCounterpart;
											traverseCounterpart->counterpart = temp;
											break;
										}
										tempBehind = tempBehind->counterpart;
									}
									dayFound = true;
									this->count++;
									added = true;
									break;
								} 
								if(!setToDay)
								{
									Task *day = new Task;
									day = this->head;
									while(day->day != newTask->day){day = day->next;}
									findDay = day;
									setToDay = true;
								}
								findDay = findDay->next;
							} while(findDay != this->head);

							if(!dayFound){
								traverseCounterpart->previous = this->head->previous;
								traverseCounterpart->next = this->head;
								this->head->previous = traverseCounterpart;
								traverseCounterpart->previous->next = traverseCounterpart;
								this->count++;
								added = true;
								break;
							}

						} else //if newTask has lower priority
						{
							this->checkAvailableNextTimesFor(newTask);
							newTask->time = usable_time;
							newTask->day = usable_day;
							bool dayFound = false;
							Task *findDay = new Task;
							findDay = traverseCounterpart;
							do
							{
								if(findDay->day == newTask->day)
								{
									Task *temp = new Task;
									Task *tempBehind = new Task;
									bool firstCheck = false;
									temp = findDay;
									tempBehind = findDay;
									while(temp->counterpart != NULL)
									{
										
										if((!firstCheck) && (newTask->time < temp->time)){
											newTask->next = temp->next;
											newTask->previous = temp->previous;
											temp->previous->next = newTask;
											temp->next->previous = newTask;
											newTask->counterpart = temp;
											temp->next = NULL;
											temp->previous = NULL;
											break;
										}
										firstCheck = true;
										temp = temp->counterpart;
										if(newTask->time < temp->time)
										{
											tempBehind->counterpart = newTask;
											newTask->counterpart = temp;
											break;
										}
										tempBehind = tempBehind->counterpart;
									}
									dayFound = true;
									this->count++;
									added = true;
									break;
								} 
								findDay = findDay->next;
							} while(findDay != this->head);

							if(!dayFound){
								newTask->previous = this->head->previous;
								newTask->next = this->head;
								this->head->previous = newTask;
								newTask->previous->next = newTask;
								this->count++;
								added = true;
								break;
							}

						}
					} else //we don't have the same time in that day
					{
						if((traverseCounterpart->next != NULL) && (newTask->time < traverseCounterpart->time))
						{
							newTask->previous = traverseCounterpart->previous;
							newTask->previous->next = newTask;
							newTask->next = traverseCounterpart->next;
							newTask->next->previous = newTask;
							newTask->counterpart = traverseCounterpart;
							traverseCounterpart->next = NULL;
							traverseCounterpart->previous = NULL;
							if(traverseCounterpart == this->head){
								this->head = newTask;
							}
							this->count++;
							added = true;
							break;
						} else if (traverseInFront == NULL){
							traverseCounterpart->counterpart = newTask;
							this->count++;
							added = true;
							break;
						} else if(newTask->time < traverseInFront->time){
							newTask->counterpart = traverseInFront;
							traverseCounterpart->counterpart = newTask;
							this->count++;
							added = true;
							break;
						} else if((traverseInFront->counterpart == NULL) && (newTask->time > traverseInFront->time)){
							traverseInFront->counterpart = newTask;
							this->count++;
							added = true;
							break;
						}
					}
					traverseCounterpart = traverseCounterpart->counterpart;
					if(added){break;}
				}
			}
			if(added){break;}
			traverse = traverse->next;
		} while ((traverse != this->head) && (!added));
		if((traverse == this->head) && (newTask->day > this->head->previous->day))
		{
			newTask->previous = this->head->previous;
			newTask->next = this->head;
			newTask->previous->next = newTask;
			this->head->previous = newTask;
			this->count++;
		}
	}
} 

Task * WorkPlan::getTask(int day, int time)
{
	Task *traverse = new Task;
	traverse = this->head;
	bool dayFound = false;
	do
	{	
		if (traverse->day == day)
		{
			dayFound = true;
			break;
		}
		
		traverse = traverse->next;
	} while (traverse != this->head);
	if (dayFound)
	{
		while(traverse->time != time){
			traverse = traverse->counterpart;
		}
		return traverse;
	}
	return NULL;
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	
	bool found = false;
	bool justSkipped = false;
	Task *dayHolder = new Task;
	Task *traverse = new Task;
	Task *traverseInFront = new Task;
	dayHolder = this->head;
	while(dayHolder->day != delayed->day){
		dayHolder = dayHolder->next;
	}
	traverse = dayHolder;
	while(traverse->time != delayed->time){
		traverse = traverse->counterpart;
	}
	traverseInFront = traverse->counterpart;
	do{
		if(traverseInFront != NULL)
		{
			delayed->time++;
			if(justSkipped){
				delayed->time--; 
				justSkipped = false;
				if(delayed->time < traverse->time){
					usable_day = dayHolder->day;
					usable_time = delayed->time;
					found = true;
				}
				delayed->time++;
			}
			if((delayed->time < traverseInFront->time) && !found){
				usable_day = dayHolder->day;
				usable_time = delayed->time;
				found = true;
			} else {
				traverseInFront = traverseInFront->counterpart;
			}
		} else{
			delayed->time = 8;
			dayHolder = dayHolder->next;
			traverse = dayHolder;
			traverseInFront = dayHolder->counterpart;
			justSkipped = true;
		}
	}while((dayHolder != this->head) && (!found));
	if(!found){
		usable_day = this->head->previous->day + 1;
		usable_time = 8;
	}
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	Task *findDay = new Task;
	findDay = this->head;
	while(findDay->day != day){findDay = findDay->next;}
	while(findDay != NULL)
	{
		Task *oldone = new Task();
		Task *newone = new Task();
		oldone = findDay;
		newone->name = new char [strlen(oldone->name)];
		newone->name = oldone->name;
		newone->priority = oldone->priority;
		int oldDay = oldone->day;
		do{
			this->checkAvailableNextTimesFor(oldone);
			oldone->day = usable_day;
			oldone->time = usable_time;
		}
		while(oldDay == usable_day);
		newone->day = usable_day;
		newone->time = usable_time;
		findDay = findDay->counterpart;
		this->remove(oldone);	
		this->add(newone);
	}
}

void WorkPlan::remove(Task *target)
{
	if(target->next != NULL){
		if((target == this->head) && (target->counterpart == NULL)) {this->head = this->head->next;} 
		else if ((target == this->head) && (target->counterpart != NULL)) {this->head = this->head->counterpart;}
		
		if(target->counterpart == NULL)
		{
			target->next->previous = target->previous;
			target->previous->next = target->next;
		} else
		{
			target->next->previous = target->counterpart;
			target->previous->next = target->counterpart;
			target->counterpart->next = target->next;
			target->counterpart->previous = target->previous;
		}
		delete target;
		
	} else 
	{
		Task *traverse = new Task;
		traverse = this->head;
		Task *traverseBehind = new Task;
		while(traverse->day != target->day){
			traverse = traverse->next;
		}
		traverseBehind = traverse;
		traverse = traverse->counterpart;
		while(traverse->time != target->time){
			traverse = traverse->counterpart;
			traverseBehind = traverseBehind->counterpart;
		}
		traverseBehind->counterpart = traverse->counterpart;
		delete traverse;
	}
	this->count--;
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
