#include <stdio.h>
#include <pthread.h>
#include "queue.h"

struct Event{
    (*function) (void*);
    void* arguments;
};

// The qeueue that holds the events that have been queued
void* eventQueue;

pthread_mutex_t eventLock;
// pthread_mutex_t threadLock;

int initEventHandler(){
    // Create the queue to add elements to later
    eventQueue = createQueue();
    if(!pthread_mutex_init(&eventLock, NULL)){
        printf("Couldn't initialize the mutex for the event handler.\n");
        return 200;
    }
}

// Frees all memory that the event handler uses
void freeEventHandler(){
    freeQueue(eventQueue);
    pthread_mutex_destroy(&eventLock);
}

void addEvent(struct Event* e){
    pthread_mutex_lock(&eventLock);
    enqueue(eventQueue, e);
    pthread_mutex_unlock(&eventLock);
}

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