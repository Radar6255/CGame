// File to draw objects
#include <stdint.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <cglm/cglm.h>

#include "../errors.h"
#include "headers/hashMap.h"

// Struct to contain an object
struct RenderObject {
    mat3 screenT;
    unsigned int id;
    char vaoIndex;
};

struct RenderArguments {
    GLuint program;
    GLuint screenTransUni;
};

// Initailize everything. Store everything in either an array or a hash set
// Returns the hash map that will be used to do rendering. Used for the rest of the functions in this file
void* intializeRenderer(int size){
    if(!size){
        // If no size is specified assume 16. We will correct later if this is wrong
        size = 16;
    }

    // Hash map of objects that we are rendering
    void* renderObjects = createHashMap(size);

    return renderObjects;
}


// Add object to render
int addRenderObject(void* renderObjects, int x, int y, char vaoIndex){
    // TODO Get the screen transformation matrix and set it
    struct RenderObject* newObject = malloc(sizeof(struct RenderObject));

    // Clearing the screen transform
    glm_mat3_identity(newObject->screenT);

    newObject->id = numHashMapElements(renderObjects);
    
    newObject->vaoIndex = vaoIndex;

    setHashMapEntry(renderObjects, newObject, newObject->id);

    return newObject->id;
}

// Remove an object to render
char removeRenderObject(void* renderObjects, unsigned int renderObjectId){
    void* renderObject = removeHashMapEntry(renderObjects, renderObjectId);
    // Checking to see if we couldn't find the specified ID in the hash map
    if (!renderObject)
        return 0;
    free(renderObject);
    return 1;
}

// Renders the specified objects from the hash map with the specified program
// Render args is a pointer to a render arguments
void renderFunc(void* renderObject, void* renderArgs){
    struct RenderArguments* RenderArgumentsS = renderArgs;
    struct RenderObject* renderObjectS = (struct RenderObject*) renderObject;

    glUseProgram(RenderArgumentsS->program);
    
    glUniformMatrix3fv(RenderArgumentsS->screenTransUni, 1, GL_FALSE, renderObjectS->screenT[0]);

    glBindVertexArray(renderObjectS->vaoIndex);
    glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT);
}

// Perform a render call
void renderAllObjects(void* renderObjects, GLuint program, GLuint screenTransUni){
    // Creating an object to store the arguments for the render function
    struct RenderArguments* args = malloc(sizeof(struct RenderArguments));
    args->program = program;
    args->screenTransUni = screenTransUni;

    hashMapSetFunc(renderObjects, renderFunc, &args);
}

// Translate an object
void translateObject(void* renderObjects, unsigned int renderObjectId, float trans[2]){
    struct RenderObject* renderObjectS = getHashMapEntry(renderObjects, renderObjectId);

    // TODO Try using just the vector instead of making a vec2. Not sure if it will work
    vec2 off;
    glm_vec2(trans, off);
    glm_translate2d(renderObjectS->screenT, off);
}

// Rotate an object
// Theta is in radians
void rotateObject(void* renderObjects, unsigned int renderObjectId, float theta){
    struct RenderObject* renderObjectS = getHashMapEntry(renderObjects, renderObjectId);

    glm_rotate2d(renderObjectS->screenT, theta);
}

// Scale object
void scaleObject(void* renderObjects, unsigned int renderObjectId, float scale[2]){
    struct RenderObject* renderObjectS = getHashMapEntry(renderObjects, renderObjectId);

    // TODO Potentially turn scale into vec2
    glm_scale2d(renderObjectS->screenT, scale);
}

// Free renderer
void freeRenderer(void* renderObjects){
    freeHashMapElements(renderObjects);
    freeHashMap(renderObjects);
}