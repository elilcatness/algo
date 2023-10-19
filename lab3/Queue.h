#include "Task.h"

const uint queueSize = 5;

#pragma once
class Queue {
	Task* q[queueSize] = {};
	int st = -1, end = -1;
	uint num;
public:
	Queue(uint);
	int push(Task* t);
	Task* pop();
	bool isEmpty();
	void display();
	~Queue();
};