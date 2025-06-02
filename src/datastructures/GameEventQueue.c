#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <datastructures/GameEventQueue.h>


typedef struct geQueue {
    char* queueArray; // A char is one byte, so I can just specify amount of bytes needed
    int64_t maxElements;
    size_t elementSize;
    size_t numArrayMembers;
    int64_t front;
    int64_t back;
} geQueue;

geQueue* geQueueCreate(size_t datatypeSizeBytes) {
    geQueue* queue = malloc(sizeof(geQueue));
    queue->queueArray = malloc(datatypeSizeBytes * GEQUEUE_INITIAL_SIZE);
    queue->maxElements = GEQUEUE_INITIAL_SIZE;
    queue->elementSize = datatypeSizeBytes;
    queue->front = -1;
    queue->back = 0;
    return queue;
}

uint8_t geQueueIsFull(geQueue* queue) {
    return (queue->back >= queue->maxElements);
}

uint8_t geQueueIsEmpty(geQueue* queue) {
    return (queue->front == queue->back - 1);
}

void geQueueEnqueue(geQueue* queue, void* data) {
    // Don't add the data if the queue is full (TODO: Convert to realloc)
    if (geQueueIsFull(queue)) {
        printf("WARNING: Queue overflow\n");
        return;
    }


    memcpy( queue->queueArray + queue->back * queue->elementSize, data, queue->elementSize);
    queue->back++;
}

void geQueueDequeue(geQueue* queue, void* dest) {
    if (geQueueIsEmpty(queue)) {
        printf("WARNING: Queue underflow\n");
        return;
    }


    queue->front++;
    memcpy(dest, queue->queueArray + queue->front * queue->elementSize, queue->elementSize);
    memset(queue->queueArray + queue->front * queue->elementSize, 0, queue->elementSize);
    // void* data = (void*)(queue->queueArray)[queue->front];
    // (queue->queueArray)[queue->front] = NULL;
}