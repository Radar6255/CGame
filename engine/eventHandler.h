#ifndef EVENT_HANDLER
#define EVENT_HANDLER

// Holds an event object
//      *function (void*) - Function to call for the event
//      void* arguments - The arguments to pass to the function call
struct Event{
    void (*function) (void* arg);
    void* arguments;
};

// Function to add an event to the queue
//      struct Event* e - The event to add to the queue of events to happen next tick
void addEvent(struct Event* e);

// Frees all memory that the event handler uses and stops the event handler
//      int out - Returns 0 if the event handler stops successfully 202 otherwise
int stopEventHandler();

// Initializes the event handler
int initEventHandler();
#endif