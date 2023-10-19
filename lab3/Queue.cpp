#include "Queue.h"
#include <iostream>

using namespace std;

Queue::Queue(uint n) { num = n; }

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

void Queue::display() {
	/*int end0 = end;
	cout << 'F' << num << ": ";
	Task* firstT = pop();
	if (firstT == nullptr)
		return;
	cout << firstT->id << ' ';
	push(firstT);
	Task* t = pop();
	while (t != firstT) {
		cout << t->id << ' ';
		push(t);
		t = pop();
	}
	push(t);
	st = 0;
	end = end0;
	cout << endl;*/
	cout << 'F' << num << ": ";
	Queue q0(num);
	Task* t = pop();
	while (t != nullptr) {
		cout << t->id << " (" << t->processed << '/' 
			<< t->duration << ") | ";
		q0.push(t);
		t = pop();
	}
	t = q0.pop();
	while (t != nullptr) {
		push(t);
		t = q0.pop();
	}
	cout << endl;

}

bool Queue::isEmpty() { return st == -1; }

Queue::~Queue() {
	Task* t = pop();
	while (t != nullptr) {
		delete t;
		t = pop();
	}
}
