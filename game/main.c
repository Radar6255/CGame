#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "glFunctions/headers/display.h"
#include "glFunctions/headers/keyboard.h"
#include "glFunctions/headers/mouse.h"
#include "glFunctions/headers/init.h"

enum RunningModes{
  GAME,
  WBUILDER
};

void printErr(char* errMsg){
    fprintf(stderr, errMsg);
    getchar();
}

// Used to keep what the engine is running at the moment
static enum RunningModes mode = GAME;

enum RunningModes getMode(){
    return mode;
}

int screenId;
Window win;
Display * dis;

int closeGame(){
    // freeGLResources();

    printf("Cleaning up resources...\n");
	XDestroyWindow(dis, win);
    // XFree()
	XCloseDisplay(dis);
    return 0;
}

int main(int argc, char** argv){
    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

    if (argc > 1){
        // TODO Possibly move this into another function if I need to do a lot of specific things for the world builder
        if (!strcmp(argv[1], "wbuilder")){
            printf("Starting world builder...\n");
            mode = WBUILDER;
        }else if(!strcmp(argv[1], "-w")){
            printf("Starting world builder...\n");
            mode = WBUILDER;
        }
    }

    dis = XOpenDisplay(NULL);
    if(!dis){
        printf("Unable to connect to X11 display! This program relies on X11 window management which is available under some linux distros.\n");
        return 101;
    }

    Window root = DefaultRootWindow(dis);
    screenId = DefaultScreen(dis);

    XVisualInfo *vi = glXChooseVisual(dis, screenId, att);
    if(vi == NULL){
        printf("Cannot make GLX window!\n");
        XCloseDisplay(dis);
        return 103;
    }

    if(!vi){
        printf("No valid display found\n");
        return 102;
    }

    Colormap cmap = XCreateColormap(dis, root, vi->visual, AllocNone);

    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    // long white = WhitePixel(dis, screen);
    // long black = BlackPixel(dis, screen);

    win = XCreateWindow(dis, root, 0, 0,	
		758, 568, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XMapWindow(dis, win);
    
    char* title;

    switch (mode) {
    case GAME:
        title = "A Game in C";
        break;
    case WBUILDER:
        title = "World Builder Tool";
        break;
    default:
        title = "TODO Create a custom title";
        break;
    }

    XStoreName(dis, win, title);

    GLint majorGLX, minorGLX;
    glXQueryVersion(dis, &majorGLX, &minorGLX);
    printf("GLX version: %d.%d\n", majorGLX, minorGLX);

    GLXContext glc = glXCreateContext(dis, vi, NULL, GL_TRUE);
    glXMakeCurrent(dis, win, glc);

    // XSetStandardProperties(dis, win, title, "C Engine", None, NULL, 0, NULL);
    
    printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GL Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Using GL Renderer Version: %s\n", glGetString(GL_RENDER));

    GLenum glewErr = glewInit();
    
    if(glewErr != GLEW_OK){
        printf("GLEW Error: %s\n", glewGetErrorString(glewErr));
        return 100;
    }
    printf("Successfully initialized GLEW.\n");

    XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);

    XEvent xev;
    XWindowAttributes gwa;
    while(1){
        XNextEvent(dis, &xev);

        if(xev.type == Expose){
            printf("Expose\n");
            XGetWindowAttributes(dis, win, &gwa);
            glViewport(0, 0, gwa.width, gwa.height);

            display();
            glXSwapBuffers(dis, win);
            // Initialization here?
        }else if(xev.type == KeyPress){
            printf("KeyPress\n");
            glXMakeCurrent(dis, None, NULL);
            // glXDestroyContext(dis, glc);
            // XDestroyWindow(dis, win);
            // XCloseDisplay(dis);
            // return 0;
        }else if(xev.type == GraphicsExpose){
            printf("GraphicsExpose\n");
        }
    }

    // TODO Figure out how to do clean up
    closeGame();

    return 0;
}