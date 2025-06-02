#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <datastructures/GameEventQueue.h>


int geQueueCreate(geQueue* dest) {
    // geQueue* newQueue = malloc(sizeof(geQueue));
    // if (newQueue == NULL) {
    //     // do something error-handly here?
    //     return 1;
    // }
    dest->head = NULL;
    dest->tail = NULL;

    // dest = newQueue;
    return 0;
}

int geQueueEnqueue(geQueue* queue, void* data, size_t size) {
    geqNode* newNode = malloc(sizeof(geqNode));

    if (newNode == NULL) {
        return 1;
    }

    newNode->data = malloc(size);

    if (newNode->data == NULL) {
        return 1;
    }

    newNode->size = size;
    memcpy(newNode->data, data, size); 

    if (queue->tail != NULL) {
        queue->tail->next = newNode;
    }
    newNode->next = NULL; 
    queue->tail = newNode;

    if (queue->head == NULL) {
        queue->head = newNode;
    }
    return 0;
}

int geQueueDequeue(geQueue* queue, void* dest) {
    if (queue->head == NULL) {
        return 1;
    }

    geqNode* deQueuedNode = queue->head;
    memcpy(dest, deQueuedNode->data, deQueuedNode->size);

    if (deQueuedNode->next == NULL) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = deQueuedNode->next;
    }

    free(deQueuedNode->data);
    free(deQueuedNode);
    return 0;
}

int geQueueIsEmpty(geQueue* queue) {
    return (queue->head == NULL);
}