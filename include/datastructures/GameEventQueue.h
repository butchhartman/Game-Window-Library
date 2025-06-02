#ifndef _GAMEQUEUE_H_
#define _GAMEQUEUE_H_
#include <stdint.h>
// How many elements of the specified type the queue will hold at once

typedef struct geqNode {
    void* data;
    size_t size;
    struct geqNode* next;
} geqNode;

typedef struct geQueue {
    geqNode* head;
    geqNode* tail;
} geQueue;

int geQueueCreate(geQueue* dest);

int geQueueEnqueue(geQueue* queue, void* data, size_t size);

int geQueueDequeue(geQueue* queue, void* dest);

int geQueueIsEmpty(geQueue* queue);

#endif // _GAMEQUEUE_H_