#include "Task.h"

const uint queueSize = 5;

#pragma once
class Queue {
	Task* q[queueSize] = {};
	int st = -1, end = -1;
public:
	int push(Task* t);
	Task* pop();
	~Queue();
};