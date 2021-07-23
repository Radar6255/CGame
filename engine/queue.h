// Creates a new queue object
void *createQueue();
// Adds an element to the queue
//      void* queue - The queue you are adding an element to
//      void* element - The element to add to the queue
void enqueue(void* queue, void* element);
// Returns the element at the bottom of the queue and dequeues it
//      void* queue - The queue to get the element of
void* dequeue(void* queue);
// Frees the queue and the elements in the queue
void freeQueue(void* queue);
// True if the queue is empty, false otherwise
char queueIsEmpty(void* queue);