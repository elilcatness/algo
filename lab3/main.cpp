#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>
#include <conio.h>
#include <iostream>
#include "Queue.h"

using namespace std;

const int keyLeft = 75;
const int keyUp = 72;
const int keyRight = 77;
const int keyDown = 80;
const int keyEnter = 13;
const int keyEsc = 27;
const int keyCtrlC = 3;
const int key0 = 48;
const int key1 = 49;
const int key2 = 50;

const char* breakTip = "Для выхода нажмите Esc или Ctrl+C";
const char* continueTip = "Для перехода на следующий такт нажмите стрелку вправо";
const char* pushTip = "Для добавления задачи нажмите стрелку вверх или Enter";

struct Stack {
	Task* task;
	Stack* next = nullptr;
};

void pushStack(Stack*& s, Task* t) {
	if (s == nullptr) {
		s = new Stack{ t };
	}
	else {
		Stack* newS = new Stack{ t };
		newS->next = s;
		s = newS;
	}
}

Task* popStack(Stack*& s) {
	if (s != nullptr) {
		Stack* cur = s;
		Task* t = s->task;
		s = s->next;
		delete cur;
		return t;
	}
	return nullptr;
}

void freeStack(Stack*& s) {
	uint count = 0;
	Stack* cur = nullptr;
	while (s != nullptr) {
		delete s->task;
		cur = s;
		s = s->next;
		delete cur;
		count++;
	}
	cout << "Удалено задач из стека: " << count << endl;
}

void displayStack(Stack*& s) {
	Stack* s0 = nullptr;
	Task* t = popStack(s);
	cout << "S: ";
	while (t != nullptr) {
		cout << t->id << " (" << t->processed << '/'
			<< t->duration << ") | ";
		pushStack(s0, t);
		t = popStack(s);
	}
	t = popStack(s0);
	while (t != nullptr) {
		pushStack(s, t);
		t = popStack(s0);
	}
	cout << endl;
}

void displayTask(Task* t) {
	cout << "Задача #" << t->id << ". Приоритет: " << t->priority
		<< ". Такт получения: " << t->clock << ". Выполнение: "
		<< t->processed << '/' << t->duration << endl;
}

void showSystem(Queue q[], const uint queueCount, Stack*& s) {
	for (uint i = 0; i < queueCount; i++)
		q[i].display();
	displayStack(s);
	cout << endl;
}

void showTips() {
	cout << continueTip << endl << pushTip
		<< endl << breakTip << "\n\n";
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

int handleButtonPress(bool& isArrow) {
	char c = _getch();
	switch (c) {
	case -32:
		isArrow = true;
		return _getch();
	default:
		isArrow = false;
		return c;
	}
}

void process() {
	Queue F[] = {Queue(0), Queue(1), Queue(2)};
	uint queueCount = sizeof(F) / sizeof(Queue);
	Stack* S = nullptr;
	Task* curTask = nullptr;
	Task* t;
	uint clock = 0, lastId = generateTasks(F), priority, duration;
	bool isArrow = false, freezeProcess = true, increment = true;
	int c;
	char p;
	while (true) {
		if (!freezeProcess) {
			if (curTask != nullptr && curTask->processed == curTask->duration) {
				delete curTask;
				curTask = nullptr;
			}
			if (curTask == nullptr) {
				t = popStack(S);
				if (t != nullptr && t->priority == 1)
					cout << "stack" << endl;
				for (uint i = 0; i < (t == nullptr ? queueCount : t->priority); i++) {
					if (!F[i].isEmpty()) {
						curTask = F[i].pop();
						break;
					}
				}
				if (t != nullptr)
					if (curTask == nullptr)
						curTask = t;
					else
						pushStack(S, t);
			}
		}
		system("cls");
		showTips();
		showSystem(F, queueCount, S);
		cout << "Такт: " << clock << endl;
		if (!freezeProcess) {
			if (curTask) {
				cout << "Текущая задача: ";
				if (increment)
					curTask->processed++;
				else
					increment = true;
				displayTask(curTask);
			}
			else
				cout << "Состояние: Процессор свободен" << endl;
		}
		else if (curTask != nullptr) {
			cout << "Последняя добавленная задача: ";
			if (curTask->id == 16)
				cout << "..." << endl;
			displayTask(curTask);
		}
		freezeProcess = false;
		c = handleButtonPress(isArrow);
		if (!isArrow && (c == keyEsc || c == keyCtrlC))
			break;
		if (isArrow && c == keyRight) {
			clock++;
		}
		else if (isArrow && c == keyUp || !isArrow && c == keyEnter) {
			freezeProcess = true;
			cout << "\nДобавление задачи..." << endl;
			cout << breakTip << endl;
			while (true) {
				cout << "Выберите приоритет задачи и нажмите на клавиатуре: 0, 1 или 2" << endl;
				c = handleButtonPress(isArrow);
				if (!isArrow && (c == keyEsc || c == keyCtrlC)) {
					break;
				}
				if (isArrow) {
					cout << "Некорректный ввод!" << endl;
					continue;
				}
				switch (c) {
				case key0:
					priority = 0;
					break;
				case key1:
					priority = 1;
					break;
				case key2:
					priority = 2;
					break;
				default:
					cout << "Некорректный ввод!" << endl;
					continue;
				}
				cout << "Выбран приоритет: " << priority << endl;
				while (true) {
					cout << "Введите время выполнения данной задачи "
						<< "в тактах: ";
					if (cin.peek() == '-')
						cin.putback('&');
					cin >> duration;
					if (cin.fail() || duration == 0 || cin.get() != '\n') {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Некорректный ввод!" << endl;
					}
					else break;
				}
				t = new Task{ ++lastId, priority, duration, clock };
				if (curTask == nullptr && clock > 0)
					curTask = t;
				else if (curTask != nullptr && priority < curTask->priority &&
					curTask->processed <= curTask->duration / 2) {
					pushStack(S, curTask);
					curTask = t;
				}
				else
					if (F[priority].push(t) == -1) {
						cout << "Очередь F" << priority << " заполнена! "
							<< "Не удалось добавить задачу" << endl;
						lastId--;
						delete t;
						system("pause");
						freezeProcess = false;
						increment = false;
					}
				break;
			}
		}
		else {
			increment = false;
		}
	}
	freeStack(S);
	if (curTask != nullptr)
		delete curTask;
	cout << "Завершение программы." << endl;
}

int main() {
	srand(static_cast<uint>(time(0)));
	process();
	cout << endl;
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
}