#include <stdio.h>
#include <assert.h>
#include <datastructures/GameEventQueue.h>

// Once size amount of objects are sent into the queue, it beocomes unusable. I need a way to have the queue be reusable. (Have the back pointer wrap around if there is less than 50 in the queue) 

int main() {
    geQueue* myQueue = geQueueCreate(sizeof(int));

    int testVal = -12345;

    // Test queue empty
    assert(geQueueIsEmpty(myQueue) == 1);

    // Test underflow checking
    geQueueDequeue(myQueue, &testVal);
    assert(testVal == -12345);

    // test overflow checking
    // should be 5 reports of overflow
    for (int i = 0; i < 55; i++) {
        geQueueEnqueue(myQueue, &i);
    }

    // test queue full
    assert(geQueueIsFull(myQueue) == 1);
    assert(geQueueIsEmpty(myQueue) == 0);

    // Test that all values can be accesssed
    for (int i = 0; i < 50; i++) {
        geQueueDequeue(myQueue, &testVal);
        assert(testVal == i);
    } 
    
    testVal = 0;

    // test that values can be added after a full queue enqueue and dequeue
    for (int i = 0; i < 50; i++) {
        geQueueEnqueue(myQueue, &i);
    }
    // test that all values can be accessed after a full queue enqueue and dequeue
    for (int i = 0; i < 50; i++) {
        geQueueDequeue(myQueue, &testVal);
        assert(testVal == i);
    }

    return 0;
}