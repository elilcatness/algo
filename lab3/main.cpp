#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include "Queue.h"

using namespace std;

struct Stack {
	Task* task;
	Stack* next = nullptr;
};

void pushStack(Stack*& s, Task* t) {
	if (s == nullptr)
		s = new Stack{ t };
	else {
		Stack* newS = new Stack{ t };
		newS->next = s;
		s = newS;
	}
}

Task* popStack(Stack*& s) {
	if (s != nullptr) {
		Task*& t = s->task;
		s = s->next;
		return t;
	}
	return nullptr;
}

void freeStack(Stack*& s) {
	uint count = 0;
	Stack* cur = nullptr;
	while (s) {
		delete s->task;
		cur = s;
		s = s->next;
		delete cur;
		count++;
	}
	cout << "Удалено задач из стека: " << count << endl;
}

void displayTask(Task* t) {
	cout << "Задача #" << t->id << ". Приоритет: " << t->priority
		<< ". Такт получения: " << t->clock << ". Выполнение: "
		<< t->processed << '/' << t->duration << endl;
}

inline uint getRand(uint a, uint b) {
	return a + rand() % b;
}

uint generateTasks(Queue F[], uint lastId = 0, uint clock = 0) {
	uint count = 0;
	Task* t;
	for (uint i = 0; i < 3; i++) {
		count = getRand(100, 200);
		for (uint j = 0; j < count; j++) {
			t = new Task{ lastId + 1, i, getRand(1, 10), clock };
			if (F[i].push(t) == -1) {
				delete t;
				break;
			}
			lastId++;
		}
	}
	return lastId;
}

void process() {
	Queue F[] = { Queue(), Queue(), Queue() };
	uint queueCount = sizeof(F) / sizeof(Queue);
	Stack* S = nullptr;
	uint clock = 0, lastId = generateTasks(F);
	Task* t;
	for (uint i = 0; i < queueCount; i++) {
		t = F[i].pop();
		while (t != nullptr) {
			displayTask(t);
			delete t;
			t = F[i].pop();
		}
	}
}

int main() {
	srand(static_cast<uint>(time(0)));
	process();
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
}