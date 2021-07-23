#include <stdio.h>
#include <stdlib.h>

struct queueS{
    struct queueElementS* start;
    struct queueElementS* end;
    int currSize;
};

struct queueElementS{
    struct queueElementS* next;
    void* data;
};

void *createQueue(){
    struct queueS* out = malloc(sizeof(struct queueS));
    out->currSize = 0;
    return out;
}

void enqueue(void* queue, void* data){
    struct queueS* queueO = (struct queueS*) queue;

    if (queueO->currSize){
        queueO->end->next = malloc(sizeof(struct queueElementS));
        queueO->end = queueO->end->next;
        queueO->end->data = data;
        queueO->end->next = NULL;
        queueO->currSize++;
        return;
    }

    queueO->start = malloc(sizeof(struct queueElementS));
    queueO->start->data = data;
    queueO->start->next = NULL;
    queueO->end = queueO->start;
    queueO->currSize++;
}

void* dequeue(void *queue){
    struct queueS* queueO = (struct queueS*) queue;
    void* out = queueO->start->data;

    queueO->currSize--;
    if(queueO->currSize){
        void *firstEntry = queueO->start;
        queueO->start = queueO->start->next;
        free(firstEntry);

        return out;
    }

    free(queueO->start);
    return out;
}

void freeQueueElements(void* firstElement){
    if(firstElement){
        struct queueElementS* qE = (struct queueElementS*) firstElement;
        freeQueueElements(qE->next);
        free(qE->data);
        free(firstElement);
    }
}

void freeQueue(void *queue){
    struct queueS* queueO = (struct queueS*) queue;

    if(queueO->currSize){
        freeQueueElements(queueO->start);
    }
    free(queue);
}

char queueIsEmpty(void* queue){
    struct queueS* queueO = (struct queueS*) queue;
    return !queueO->currSize;
}