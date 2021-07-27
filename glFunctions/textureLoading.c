#include <stdlib.h>

#include <dirent.h>
#include <regex.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "headers/stb_image.h"

#define IMAGE_ASSET_LOCATION "../assets"
#define IMAGE_FORMAT_REGEX ".png"

int filter(const struct dirent* dir){
    if (!dir){
        return 0;
    }
    
    regex_t regex;
    if (regcomp(&regex, IMAGE_FORMAT_REGEX, 0)){
        printf("ERROR: Couldn't compile regex for finding image files\n");
        exit(203);
    }

    return regexec(&regex, dir, 0, NULL, 0);
}

// TODO Change to load the images in an order or have some way to pull them by name not alphabetically
GLuint* initializeTextures(){
    struct dirent **namelist;

    int numImages = scandirat(IMAGE_ASSET_LOCATION, namelist, filter, alphasort);

    GLuint* out = (GLuint*) malloc(sizeof(GLuint) * numImages);

    for(int i = 0; i < numImages; i++){
        int width, height, nChannels;
        unsigned char *imageData = stbi_load(namelist[i], &width, &height, &nChannels, 0);
        if(!imageData){
            printf("Failed to load image '%s'. Continuing if possible...\n", namelist[i]);
        }

        glGenTextures(1, &(out[i]));
        glBindTexture(GL_TEXTURE_2D, out[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

        stbi_image_free(imageData);
    }

    return out;
}