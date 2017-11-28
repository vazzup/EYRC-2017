#ifndef Q_EY
#define Q_EY

#define N 200

int count = 0;
int queue[N];
int head = -1, tail = -1;

void init_queue(void) {
	int i;
	for(i=0; i<N; i++) {
		queue[i] = -1;
	}
}

void reset_queue(void) {
	init_queue();
	head = -1;
	tail = -1;
	count = 0;
}

int is_empty(void) {
	return ((count == 0)? 1 : 0);
}

int peek(void) {
	if(count == 0) {
		return -1;
	} else {
		return queue[head + 1];
	}
}

void enqueue(int x) {
	if(count == N) {
		return;
	} else {
		++tail;
		tail %= N;
		queue[tail] = x;
		count++;
	}
}

void dequeue(void) {
	if(count == 0) {
		return;
	} else {
		++head;
		head %= N;
		count -= 1;
	}
}

#endif