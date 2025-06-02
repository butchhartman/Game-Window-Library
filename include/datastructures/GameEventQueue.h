#ifndef _GAMEQUEUE_H_
#define _GAMEQUEUE_H_
#include <stdint.h>
// How many elements of the specified type the queue will hold at once
#define GEQUEUE_INITIAL_SIZE 50

typedef struct geQueue geQueue;

geQueue* geQueueCreate(size_t datatypeSizeBytes);

uint8_t geQueueIsFull(geQueue* queue);

uint8_t geQueueIsEmpty(geQueue* queue);

void geQueueEnqueue(geQueue* queue, void* data);

void geQueueDequeue(geQueue* queue, void* dest);

#endif // _GAMEQUEUE_H_