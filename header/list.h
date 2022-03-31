extern void listAppend(void* in, void* element);
extern void* createList(int maxSize);
extern void* getListElement(void* in, unsigned int index);
extern void freeList(void* in);
extern void freeElements(void* in);