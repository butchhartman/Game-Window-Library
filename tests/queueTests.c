#include <stdio.h>
#include <assert.h>
#include <datastructures/GameEventQueue.h>

void queueue(geQueue* queue) {
    int qVal = 3456543;
    geQueueEnqueue(queue, &qVal, sizeof(int));
}

int main() {
    // Test queueing into an empty queue and dequeueing the last item in a queue
    int testEnqueueVal = 69420;
    int testDequeueVal = -12345;
    geQueue myQ;
    geQueueCreate(&myQ);

    geQueueEnqueue(&myQ, &testEnqueueVal, sizeof(int));
    geQueueDequeue(&myQ, &testDequeueVal);

    assert(testDequeueVal == testEnqueueVal);
    assert(geQueueIsEmpty(&myQ));

    // Test queueing and dequeueing correctness after an arbitrary amount (and on an already used queue) (Also testing if the data in the queue is actually persistant)
    for (int i = 0; i < 250; i++) {
        geQueueEnqueue(&myQ, &i, sizeof(int));
    }

    assert(!geQueueIsEmpty(&myQ));

    for (int i = 0; i < 250; i++) {
        geQueueDequeue(&myQ, &testDequeueVal);
        assert(testDequeueVal == i);
    }

    assert(geQueueIsEmpty(&myQ));

    // test queueing from a different function
    queueue(&myQ);
    geQueueDequeue(&myQ, &testDequeueVal);
    assert(testDequeueVal == 3456543);
    return 0;
}