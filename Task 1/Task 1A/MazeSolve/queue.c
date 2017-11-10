#ifndef VQ_
#define VQ_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 200

typedef struct Queue {
    int queue[N];
    int head, tail;
} Queue;

void init_queue(Queue *q) {
    q -> head = -1, q -> tail = -1;
    for(int i = 0; i < N; i++) {
        (q -> queue)[i] = -1;
    }
}

void print_queue(Queue *q) {
    for(int i = (q -> head) + 1; i <= (q -> tail); i = (i + 1) % N) {
        printf("%d ", (q -> queue)[i]);
    }
    printf("\n");
}
int is_empty(Queue *q) {
    if(q -> head == q -> tail) {
        return 1;
    }
    return 0;
}
int peek(Queue *q) {
    if(q -> head == q -> tail) {
        return -1;
    } else {
        return (q -> queue)[(q -> head) + 1];
    }
}

int enqueue(Queue *q, int x) {
    if(q -> tail == (((q -> head) - 1 + N) % N)) {
        return 1;
    } else {
        q -> tail += 1;
        q -> tail %= N;
        (q -> queue)[q -> tail] = x;
        return 0;
    }
}

int dequeue(Queue *q) {
    if(q -> tail == q -> head) {
        return -1;
    } else {
        q -> head += 1;
        q -> head %= N;
        return 0;
    }
}

#endif // VQ_
