// Created by Riley Adams

#include <stdio.h>
#include <stdlib.h>

// #define HASH_TESTING
#define EXPANDING_FACTOR 2
#define RESIZE_PERCENT 0.90f

struct hashEntryS{
    void *value;
    unsigned int hash;
    char init;
};

struct hashMapS{
    // The max number of elements that can currently fit in the map
    int size;
    // The number of entries in the map
    int entryCount;
    // The entries in the map
    struct hashEntryS *entries;
};

// Returns a hashMapS*
void *createHashMap(int size){
    struct hashMapS *out = (struct hashMapS *) malloc(sizeof(struct hashMapS));

    out->size = size;
    out->entries = (struct hashEntryS *) malloc(sizeof(struct hashEntryS) * size);

    for(int i = 0; i < size; i++){
        out->entries[i].init = 0;
    }

    return out;
}

// Adds and entry to the HashMap
// Map is the map to add the value to (hashMapS*)
// Value is the pointer to the value you want to add
// Hash is the hash value of the value is given
void setHashMapEntry(void* map, void *value, unsigned int hash){
    struct hashMapS* mapS = (struct hashMapS *) map;
    mapS->entryCount++;

    if(mapS->entryCount >= mapS->size * RESIZE_PERCENT){
        mapS->size = mapS->size * EXPANDING_FACTOR;
        mapS->entries = realloc(mapS->entries, mapS->size * EXPANDING_FACTOR);
    }

    int hashOff = hash;
    // Loops until it finds an empty slot to put the entry into or it finds the hash entry
    while(mapS->entries[hashOff % mapS->size].init == 1) {
        // Checking to see if we already have the entry. If we do then break and put the information there
        if (mapS->entries[hashOff % mapS->size].hash == hash) {
            break;
        }
        hashOff++;
    }
    mapS->entries[hashOff % mapS->size].hash = hash;
    mapS->entries[hashOff % mapS->size].value = value;
    mapS->entries[hashOff % mapS->size].init = 1;
}

// Removes the specified hash entry from the hash map
// Returns the value that was stored at the hash
// If it wasn't in the hash map returns a null pointer
void* removeHashMapEntry(void* map, unsigned int hash){
    struct hashMapS* mapS = (struct hashMapS *) map;
    
    int hashOff = hash;

    while(mapS->entries[hashOff % mapS->size].init == 1 && hashOff < mapS->size * 2) {
        if (mapS->entries[hashOff % mapS->size].hash == hash) {
            mapS->entries[hashOff % mapS->size].init = 0;
            return mapS->entries[hashOff % mapS->size].value;
        }
        hashOff++;
    }

    return NULL;
}

// Gets the value from a hash map entry
// If the entry doesn't exist it returns a NULL pointer
void* getHashMapEntry(void* map, unsigned int hash){
    struct hashMapS* mapS = (struct hashMapS *) map;

    int hashOff = hash;
    // Loop until you find the correct hash or run into an unitialized entry.
    // We are gauranteed to run into an unitialized entry because the map will never be completely full.
    // We resize the map if it is more that 90% full
    while(mapS->entries[hashOff % mapS->size].hash != hash
        && mapS->entries[hashOff % mapS->size].init == 1){ hashOff++; }

    // Find if you found the correct hash or an uninitialized entry
    if(mapS->entries[hashOff % mapS->size].init
        && mapS->entries[hashOff % mapS->size].hash == hash){
        return mapS->entries[hashOff % mapS->size].value;
    }
    // If we can't find an entry just return null
    return NULL;
}

// Returns a char/boolean of whether the has is in the map or not
char hashMapContains(void* map, unsigned int hash){
    struct hashMapS* mapS = (struct hashMapS *) map;

    int hashOff = hash;
    // Loop until you find the correct hash or run into an unitialized entry.
    // We are gauranteed to run into an unitialized entry because the map will never be completely full.
    // We resize the map if it is more that 90% full
    while(mapS->entries[hashOff % mapS->size].hash != hash
        && mapS->entries[hashOff % mapS->size].init == 1){ hashOff++; }

    // Find if you found the correct hash or an uninitialized entry
    if(mapS->entries[hashOff % mapS->size].init
        && mapS->entries[hashOff % mapS->size].hash == hash){
        return 1;
    }
    return 0;
}

// Loops over all of the initailized items in the hash map and performs the function on them
// The specified function takes in the value as a void* and returns void
void hashMapSetFunc(void* map, void (*function)(void* value, void* args), void* args){
    struct hashMapS* mapS = (struct hashMapS *) map;

    for(int hash = 0; hash < mapS->size; hash++){
        if(mapS->entries[hash].init){
            function(mapS->entries[hash].value, args);
        }
    }
}

// Frees the hashmap
void freeHashMap(void* map){
    struct hashMapS* mapS = (struct hashMapS *) map;

    free(mapS->entries);
    free(mapS);
}

// Frees all hash map entries if the values are safe to free
void freeHashMapElements(void *map){
    struct hashMapS* mapS = (struct hashMapS *) map;

    for(int i = 0; i < mapS->size; i++){
        if(mapS->entries[i].init){
            free(mapS->entries[i].value);
        }
    }

    free(mapS->entries);
    free(mapS);
}

// Returns the number of elements currently in the hash map
int numHashMapElements(void *map){
    struct hashMapS* mapS = (struct hashMapS *) map;

    return mapS->entryCount;
}

#ifdef HASH_TESTING
// Running tests for the hashMap functions
int main(void){
    void *map = createHashMap(4);

    int t1 = 10;
    int t2 = 0;
    int t3 = 3;
    int t4 = 4;
    int t5 = 5;

    setHashMapEntry(map, &t1, t1);
    setHashMapEntry(map, &t2, t2);

    printf("%d\n", hashMapContains(map, t1));
    printf("%d\n", hashMapContains(map, t2));
    printf("%d\n", hashMapContains(map, 1) == 0);

    setHashMapEntry(map, &t3, t3);
    setHashMapEntry(map, &t4, t4);

    printf("%d\n", hashMapContains(map, t3));
    printf("%d\n", hashMapContains(map, t4));

    printf("%d\n", *(int*) removeHashMapEntry(map, t4) == 4);

    printf("%d\n", hashMapContains(map, t4) == 0);
    printf("%d\n", getHashMapEntry(map, t4) == NULL);
    printf("%d\n", *(int*) getHashMapEntry(map, t3) == t3);

    freeHashMap(map);
}
#endif