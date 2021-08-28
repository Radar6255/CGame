#include <stdio.h>
#include <pthread.h>

#include "headers/queue.h"
#include "headers/eventHandler.h"

// How many event ticks per second
static char TICKS_PER_SECOND = 30;

// The qeueue that holds the events that have been queued
void* eventQueue;

char running = 1;
pthread_mutex_t eventLock;
// pthread_mutex_t threadLock;

pthread_t mainEventThread;

// Frees all memory that the event handler uses and stops the event handler
int stopEventHandler(){
    running = 0;
    freeQueue(eventQueue);
    pthread_mutex_destroy(&eventLock);
    int thread_return = pthread_join(mainEventThread, NULL);
    if(thread_return){
        return 202;
    }
    return 0;
}

// Function to add an event to the queue
//      struct Event* e - The event to add to the queue of events to happen next tick
void addEvent(struct Event* e){
    pthread_mutex_lock(&eventLock);
    enqueue(eventQueue, e);
    pthread_mutex_unlock(&eventLock);
}

// Function to run the events currently in the eventQueue
void runEvents(){
    // A temporary queue to hold all of the events to run this tick
    void* tempQueue = eventQueue;
    
    pthread_mutex_lock(&eventLock);
    eventQueue = createQueue();
    pthread_mutex_unlock(&eventLock);

    // TODO Run events on different threads
    while(!queueIsEmpty(tempQueue)){
        struct Event* e = (struct Event*) dequeue(tempQueue);
        e->function(e->arguments);
    }
}

// The main running loop for the event handler
void* eventHandlerLoop(void *unused){
    while(running){
        // Getting the time we started running events
        struct timespec start;
        timespec_get(&start, TIME_UTC);

        runEvents();

        // Find out how long to wait before trying to do another round of events
        struct timespec end;
        timespec_get(&end, TIME_UTC);

        end.tv_sec = 0;
        end.tv_nsec = (long)  (1000000000 / TICKS_PER_SECOND) - (end.tv_nsec - start.tv_nsec);
        nanosleep(&end, &end);
    }

    return NULL;
}

// Initializes the event handler
int initEventHandler(){
    // Create the queue to add elements to later
    eventQueue = createQueue();
    if(!pthread_mutex_init(&eventLock, NULL)){
        printf("Couldn't initialize the mutex for the event handler.\n");
        return 200;
    }

    pthread_create(&mainEventThread, NULL, &eventHandlerLoop, NULL);
    return 0;
}