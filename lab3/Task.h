typedef unsigned int uint;

struct Task {
	uint id;
	uint priority;
	uint duration;
	uint clock;
	uint processed = 0;
};