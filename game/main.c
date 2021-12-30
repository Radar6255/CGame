#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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

// What the desired max framerate is
double maxFramerate = 1000000000 / 144;

int screenId;
Window win;
Display * dis;

pthread_mutex_t resizeMutex;
int resize = 1;

void doResize(){
    pthread_mutex_lock(&resizeMutex);
    resize = 1;
    pthread_mutex_unlock(&resizeMutex);
}

int getResize(){
    pthread_mutex_lock(&resizeMutex);
    int out = resize;
    resize = 0;
    pthread_mutex_unlock(&resizeMutex);
    return out;
}

int closeGame(){
    // freeGLResources();

    printf("Cleaning up resources...\n");
	XDestroyWindow(dis, win);
    // XFree()
	XCloseDisplay(dis);
    return 0;
}

void* handleEvents(void* unused){
    XEvent xev;
    while(1){
        XNextEvent(dis, &xev);

        // XEventsQueued(display, QueuedAlready) should return the events that are still waiting to be handled 
        switch (xev.type) {
        case Expose:
            doResize();
            break;
        case KeyPress:
            printf("KeyPress\n");
            glXMakeCurrent(dis, None, NULL);

            printf("%d", xev.xkey.keycode);
            keyboard(xev.xkey.keycode, xev.xkey.x, xev.xkey.y);
            // TODO Code to close everything, not sure where to put this
            // glXDestroyContext(dis, glc);
            // XDestroyWindow(dis, win);
            // XCloseDisplay(dis);
            // return 0;
            break;
        // https://tronche.com/gui/x/xlib/events/types.html

        case ResizeRequest:
            doResize();
            break;

        default:
            break;
        }
    }
    return NULL;
}

int main(int argc, char** argv){
    if (XInitThreads()){
        printf("Successfully initialized X11 threads...\n");
    }else{
        printf("Unable to initialize X11 threads, exiting...\n");
        return 105;
    }

    if(pthread_mutex_init(&resizeMutex, NULL)){
        printf("Couldn't initialize the mutex for the resize mutex.\n");
        return 203;
    }

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

    XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask|KeyReleaseMask|FocusChangeMask|ResizeRedirectMask|PointerMotionMask);

    XWindowAttributes gwa;

    pthread_t inputThread;

    // Setting up input on a different thread
    pthread_create(&inputThread, NULL, &handleEvents, NULL);

    while(1){

        // Getting the start time of this frame used to set the max framerate properly
        // struct timespec start;
        // timespec_get(&start, TIME_UTC);
        if(getResize()){
            XGetWindowAttributes(dis, win, &gwa);
            reshape(gwa.width, gwa.height);
        }

        // Creating a new frame to display
        display();

        // Find out how long to wait before trying to call for another frame
        // struct timespec end;
        // timespec_get(&end, TIME_UTC);

        // end.tv_sec = 0;
        // end.tv_nsec = (long) (maxFramerate - (end.tv_nsec - start.tv_nsec));
        
        
        // struct timespec start1;
        // timespec_get(&start1, TIME_UTC);
        // We don't time this
        // Switch the frames and display the new one
        glXSwapBuffers(dis, win);

        // struct timespec end1;
        // timespec_get(&end1, TIME_UTC);
        // printf("Swap buffer time: %ld, %ld\n", end1.tv_sec - start1.tv_sec, end1.tv_nsec - start1.tv_nsec);

        // if(end.tv_nsec < 0){
        //     printf("Falling behind %ld...\n", end.tv_nsec);
        // }
        // nanosleep(&end, &end);
    }

	// Cleanup GLX
	glXDestroyContext(dis, glc);

    XFree(vi);
    XFreeColormap(dis, gwa.colormap);
    // TODO Figure out how to do clean up
    closeGame();

    return 0;
}