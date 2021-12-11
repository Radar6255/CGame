void *createHashMap(int size);
void setHashMapEntry(void* map, void *value, unsigned int hash);
void* removeHashMapEntry(void* map, unsigned int hash);
void hashMapSetFunc(void* map, void (*function)(void* value, void* args), void* args);
void* getHashMapEntry(void* map, unsigned int hash);
char hashMapContains(void* map, unsigned int hash);
int numHashMapElements(void *map);
void freeHashMap(void* map);
void freeHashMapElements(void *map);