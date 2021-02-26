/* @Author
Student Name: Uzay Uysal
Student ID: 150180039
Date: 10.10.2019 */

// ShoeStocker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include "node.h"
#include "stock.h"

using namespace std;

void decide(int, stock &);

int main(int argc, char* argv[]){
	
	//Open the file and check errors.
	fstream InFile;
	InFile.open(argv[1]);
	if (!InFile.is_open()) {
		cout << "A problem occured while opening the file." << endl;
		EXIT_FAILURE;
	}
	//cout << "File opened succesfully." << endl;
	
	//Creating the stock.
	stock ShoeStock;
	ShoeStock.create();

	//Taking the numbers in one by one and passing it to the main function that does everything.
	string line;
	string token;
	stringstream iss;
	int number;
	while (InFile) {
		while (getline(InFile, line)) {
			iss << line;
			while (getline(iss, token, ' ')) {
				number = atoi(token.c_str());
				decide(number, ShoeStock);
				/*I was about to use these but then noticed SSH doesn't support stoi unless I use the
				-std=c++11 flag which also does not recognize the exceptions.

				try {
					number = stoi(token);
					decide(number, ShoeStock);
				}
				catch (invalid_argument) {
					cout << "Invalid number." << endl;
				}
				catch (out_of_range) {
					cout << "Out of range." << endl;
				}
				catch (string err_desc) {
					cout << err_desc << endl;
				}*/
			}
			iss.clear();
		}
	}

	//Delete the stock at the end.
	ShoeStock.clear();

	EXIT_SUCCESS;
}

//Decide what to do based on the value of the number.
void decide(int number, stock &varStock) {

	if (number > 0){
		varStock.add_stock(number);
	}
	else if (number == 0){
		varStock.current_stock();
	}
	else {
		varStock.sell(number);
	}
}

//stock functions.
void stock::create(){
	head = NULL;
	tail = head;
	length = 0;
}

void stock::add_stock(int number){
	//First time creating a node.
	if (this->length == 0) {
		node* newNode = new node();
		this->head = newNode;
		newNode->size = number;
		newNode->quant = 1;
		newNode->next = NULL;
		this->tail = this->head;
		this->length++;
		return;
	}
	//If smaller than head add to the beginning.
	if (number < this->head->size) {
		node* newNode = new node();
		newNode->size = number;
		newNode->quant = 1;
		newNode->next = this->head;
		this->head = newNode;
		this->length++;
		return;
	}
	//If bigger than tail add to the end.
	if (number > this->tail->size) {
		node* newNode = new node();
		newNode->size = number;
		newNode->quant = 1;
		newNode->next = NULL;
		this->tail->next = newNode;
		this->tail = newNode;
		this->length++;
		return;
	}

	node* iteratorPtr;
	iteratorPtr = this->head;
	//Check if a node has the same size value.
	//If not then find the correct place and add a new node.
	while (iteratorPtr != NULL) {
		if (iteratorPtr->size == number) {
			iteratorPtr->quant++;
			return;
		}
		else if (number < iteratorPtr->next->size) {
			node* newNode = new node();
			newNode->size = number;
			newNode->quant = 1;
			newNode->next = iteratorPtr->next;
			iteratorPtr->next = newNode;
			this->length++;
			return;
		}
		iteratorPtr = iteratorPtr->next;
	}

};

//Iterate over every node and print the value.
void stock::current_stock() {
	node* iteratorPtr;
	iteratorPtr = this->head;
	while (iteratorPtr != NULL) {
		cout << iteratorPtr->size << ':' << iteratorPtr->quant << endl;
		iteratorPtr = iteratorPtr->next;
	}
}

//Do a linear search and delete the sold shoe.
void stock::sell(int number) {
	number *= -1;
	node* iteratorPtr;
	iteratorPtr = this->head;
	//Don't forget to check the head value.
	if (iteratorPtr->size == number) {
		if (iteratorPtr->quant == 1) {
			node* holderPtr;
			holderPtr = iteratorPtr;
			iteratorPtr = iteratorPtr->next;
			this->head = iteratorPtr;
			delete holderPtr;
			this->length--;
			return;
		}
		else {
			iteratorPtr->quant--;
			return;
		}

	}

	while ((iteratorPtr->next != NULL) && (iteratorPtr->next->size != number)) {
		iteratorPtr = iteratorPtr->next;
	}
	if (iteratorPtr->next != NULL) {
		if (iteratorPtr->next->quant == 1) {
			if (iteratorPtr->next->next == NULL) { this->tail = iteratorPtr; }
			node* holderPtr;
			holderPtr = iteratorPtr->next;
			iteratorPtr->next = holderPtr->next;
			delete holderPtr;
			this->length--;
			return;
		}
		else {
			iteratorPtr->next->quant--;
			return;
		}
	}
	else {
		cout << "NO_STOCK" << endl;
		return;
	}
}

void stock::clear() {
	node* holderPtr;
	while(this->head != NULL){
		holderPtr = this->head;
		this->head = this->head->next;
		delete holderPtr;
		this->length--;
	}
	this->tail = NULL;
}