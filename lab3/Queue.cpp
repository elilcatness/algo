#include "Queue.h"

int Queue::push(Task* t) {
	if ((end + 1) % queueSize == st)
		return -1; // queue overflow
	if (st == -1)
		st++;
	end = (end + 1) % queueSize;
	q[end] = t;
	return 0;
}

Task* Queue::pop() {
	if (st == -1)
		return nullptr;
	bool reset = st == end;
	Task* t = q[st];
	q[st] = nullptr;
	st = (st + 1) % queueSize;
	if (reset)
		st = end = -1;
	return t;
}

Queue::~Queue() {
	Task* t = pop();
	while (t != nullptr) {
		delete t;
		t = pop();
	}
}
