// This file has all of the code load automatically load all assets from the assets folder.
// The limit of this is that the max path size can't be more than 255

#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#include <dirent.h>
#include <regex.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "headers/texturesList.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#define IMAGE_ASSET_LOCATION "assets"
#define IMAGE_FORMAT_REGEX ".png"

// Used to find if a file has a image extension
// Used to gather all of the assets
regex_t regex;
// Used to keep track of how many images have been loaded so it can insert them into the right place
int numLoadedImages = 0;
// Holds the gl texture locations so that they can be referenced other places in the program
GLuint* glTextureLocations;

int filter(const struct dirent* dir){
    if (!dir){
        return 0;
    }
    
    return !regexec(&regex, dir->d_name, 0, NULL, 0) || dir->d_type == DT_DIR;
}

// Adds two strings together, puts the second string after the first assuming there is space
void concatString(char* first, char* second){
    char c;
    int i = -1;
    do{
        i++;
        c = first[i];
    }while(c);

    first[i] = '/';
    i++;

    int i2 = 0;
    while(second[i2]){
        first[i] = second[i2];
        i++;
        i2++;
    }
    first[i] = 0;
}

// Clones the source string to the destination given the destination is large enough
void cloneString(char* src, char* dest){
    int i = 0;
    while((dest[i] = src[i])){
        i++;
    }
}

// Returns the position of the null character in the given string
int stringSize(char *str){
    int i = 0;
    while(str[i]){
        i++;
    }
    return i;
}

// Cut string
void cutString(char* str, int pos){
    str[pos] = 0;
}

void getAllImageFiles(char* directory){
    struct dirent **fileList; 

    int numFiles = scandir(directory, &fileList, filter, alphasort);

    for(int i = 0; i < numFiles && i < 2; i++){
        free(fileList[i]);
    }
    for(int i = 2; i < numFiles; i++){
        if(fileList[i]->d_type == DT_DIR){
            char directoryPath[PATH_MAX];
            concatString(fileList[i]->d_name, directoryPath);
            // TODO get all of the image files into the output
            getAllImageFiles(directoryPath);
        }else{
            glGenTextures(1, &(glTextureLocations[numLoadedImages]));
            glBindTexture(GL_TEXTURE_2D, glTextureLocations[numLoadedImages]);
            numLoadedImages++;

            // Getting the path to the image file in a string
            char directoryPath[PATH_MAX];
            cloneString(directory, directoryPath);
            concatString(directoryPath, fileList[i]->d_name);

            // Loading the file to put into OpenGL
            int width, height, nChannels;
            unsigned char *imageData = stbi_load(directoryPath, &width, &height, &nChannels, 0);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            stbi_image_free(imageData);
        }
        free(fileList[i]);
    }
}

// TODO Change to load the images in an order
GLuint* initializeTextures(){
    // Setting up regex for the filter so I only have to do it once
    if (regcomp(&regex, IMAGE_FORMAT_REGEX, 0)){
        printf("ERROR: Couldn't compile regex for finding image files\n");
        exit(203);
    }

    glTextureLocations = (GLuint*) malloc(sizeof(GLuint) * NUM_ASSETS);
    getAllImageFiles(IMAGE_ASSET_LOCATION);
    return glTextureLocations;
}