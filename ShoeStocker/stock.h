/* @Author
Student Name: Uzay Uysal
Student ID: 150180039
Date: 10.10.2019 */

#include "node.h"

struct stock{
int length;
node* head;
node* tail;
void create();
void add_stock(int);
void sell(int);
void current_stock();
void clear();
};
