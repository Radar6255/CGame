#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#include <dirent.h>
#include <regex.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#define IMAGE_ASSET_LOCATION "assets/world/Monolith"
#define IMAGE_FORMAT_REGEX ".png"

regex_t regex;

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

// TODO Find a good way to make a good list of all of the image files in the assets folder
struct dirent **getAllImageFiles(char* directory){
    struct dirent **out;

    int numFiles = scandir(directory, &out, filter, alphasort);

    for(int i = 2; i < numFiles; i++){
        if(out[i]->d_type == DT_DIR){
            char directoryPath[PATH_MAX];
            concatString(out[i]->d_name, directoryPath);
            getAllImageFiles(directoryPath);
        }
    }
}

// TODO Change to load the images in an order or have some way to pull them by name not alphabetically
GLuint* initializeTextures(){
    struct dirent **namelist;

    // Setting up regex for the filter so I only have to do it once
    if (regcomp(&regex, IMAGE_FORMAT_REGEX, 0)){
        printf("ERROR: Couldn't compile regex for finding image files\n");
        exit(203);
    }

    int numImages = scandir(IMAGE_ASSET_LOCATION, &namelist, filter, alphasort);

    GLuint* out = (GLuint*) malloc(sizeof(GLuint) * numImages);

    char imagePath[PATH_MAX];
    cloneString(IMAGE_ASSET_LOCATION, imagePath);
    int dirLength = stringSize(IMAGE_ASSET_LOCATION);

    for(int i = 0; i < numImages; i++){
        int width, height, nChannels;
        cutString(imagePath, dirLength);
        concatString(imagePath, namelist[i]->d_name);
        unsigned char *imageData = stbi_load(imagePath, &width, &height, &nChannels, 0);

        if(!imageData){
            printf("Failed to load image '%s'. Continuing if possible...\n", imagePath);
        }

        glGenTextures(1, &(out[i]));
        glBindTexture(GL_TEXTURE_2D, out[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

        free(namelist[i]);
        stbi_image_free(imageData);
    }

    return out;
}