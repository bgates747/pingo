#include "linux_window_backend.h"

#include "render/renderer.h"
#include "render/texture.h"
#include "render/pixel.h"
#include "render/depth.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

Vec4i rect;
Vec2i totalSize;

PingoDepth *zetaBuffer;
Pixel *framebuffer;

Display *dis = 0;
int screen;
Window win;
GC gc;
XImage *img = 0;
Visual *visual;

GLXContext glContext;

PFNGLXGETVIDEOSYNCSGIPROC glXGetVideoSyncSGI;
PFNGLXWAITVIDEOSYNCSGIPROC glXWaitVideoSyncSGI;

void init_x() {
    if (dis != 0)
        return;

    dis = XOpenDisplay((char *)0);
    if (!dis) {
        fprintf(stderr, "Cannot connect to X server\n");
        exit(1);
    }

    printf("Connected to X server\n");

    screen = DefaultScreen(dis);
    unsigned long black = BlackPixel(dis, screen);
    unsigned long white = WhitePixel(dis, screen);

    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(dis, 0, att);
    if (vi == NULL) {
        fprintf(stderr, "No appropriate visual found\n");
        exit(1);
    }

    printf("Visual chosen\n");

    Colormap cmap = XCreateColormap(dis, RootWindow(dis, vi->screen), vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    win = XCreateWindow(dis, RootWindow(dis, vi->screen), 0, 0, totalSize.x, totalSize.y, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    if (!win) {
        fprintf(stderr, "Failed to create window\n");
        exit(1);
    }

    XMapWindow(dis, win);
    XStoreName(dis, win, "My Window");

    printf("Window created and mapped\n");

    glContext = glXCreateContext(dis, vi, NULL, GL_TRUE);
    if (!glContext) {
        fprintf(stderr, "Failed to create GL context\n");
        exit(1);
    }

    if (!glXMakeCurrent(dis, win, glContext)) {
        fprintf(stderr, "Failed to make GL context current\n");
        exit(1);
    }

    printf("GL context created and made current\n");

    glXGetVideoSyncSGI = (PFNGLXGETVIDEOSYNCSGIPROC)glXGetProcAddress((const GLubyte *)"glXGetVideoSyncSGI");
    glXWaitVideoSyncSGI = (PFNGLXWAITVIDEOSYNCSGIPROC)glXGetProcAddress((const GLubyte *)"glXWaitVideoSyncSGI");

    if (!glXGetVideoSyncSGI || !glXWaitVideoSyncSGI) {
        fprintf(stderr, "VSync functions not supported\n");
    } else {
        printf("VSync functions loaded\n");
    }

    gc = XCreateGC(dis, win, 0, 0);
    visual = vi->visual;

    printf("X11 initialization complete\n");
}

void enableVsync(Display *display, GLXDrawable drawable) {
    typedef void (*glXSwapIntervalEXTProc)(Display*, GLXDrawable, int);
    glXSwapIntervalEXTProc glXSwapIntervalEXT = (glXSwapIntervalEXTProc)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");
    if (glXSwapIntervalEXT) {
        glXSwapIntervalEXT(display, drawable, 1);
        printf("Enabled VSync using glXSwapIntervalEXT\n");
    } else {
        fprintf(stderr, "glXSwapIntervalEXT not supported\n");
    }
}

void init(Renderer *ren, Backend *backend, Vec4i _rect) {
    rect = _rect;
    init_x();
    LinuxWindowBackend *this = (LinuxWindowBackend *)backend;
    enableVsync(this->display, this->window);
}

void beforeRender(Renderer *ren, Backend *backend) {
    // Perform any pre-rendering setup here
}

XImage *create_ximage(Display *display, Visual *visual, int width, int height) {
    return XCreateImage(display, visual, 24, ZPixmap, 0, (char*)&framebuffer[0], width, height, 32, 0);
}

void afterRender(Renderer *ren, Backend *backend) {
    if (!img) {
        img = create_ximage(dis, visual, totalSize.x, totalSize.y);
    }

    if (glXGetVideoSyncSGI && glXWaitVideoSyncSGI) {
        unsigned int count;
        glXGetVideoSyncSGI(&count);
        glXWaitVideoSyncSGI(2, (count + 1) % 2, &count);
    }

    XEvent event;
    XNextEvent(dis, &event);
    {
        XClearArea(dis, win, 0, 0, 1, 1, true);
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, totalSize.x, totalSize.y);
        XFlush(dis);
    }

    glXSwapBuffers(dis, win);
    printf("Buffer swapped\n");
}

Pixel *getFramebuffer(Renderer *ren, Backend *backend) {
    return framebuffer;
}

PingoDepth *getZetaBuffer(Renderer *ren, Backend *backend) {
    return zetaBuffer;
}

void linuxWindowBackendInit(LinuxWindowBackend *this, Vec2i size) {
    totalSize = size;
    this->backend.init = &init;
    this->backend.beforeRender = &beforeRender;
    this->backend.afterRender = &afterRender;
    this->backend.getFramebuffer = &getFramebuffer;
    this->backend.getZetaBuffer = &getZetaBuffer;
    this->backend.drawPixel = 0;

    zetaBuffer = malloc(size.x * size.y * sizeof(PingoDepth));
    framebuffer = malloc(size.x * size.y * sizeof(Pixel));

    printf("Backend initialization complete\n");
}

int getRefreshRate(LinuxWindowBackend *backend) {
    XRRScreenConfiguration *conf = XRRGetScreenInfo(backend->display, RootWindow(backend->display, 0));
    if (!conf) {
        fprintf(stderr, "Unable to get screen info\n");
        return -1;
    }

    short refresh_rate = XRRConfigCurrentRate(conf);
    XRRFreeScreenConfigInfo(conf);

    return refresh_rate;
}

int setupVSyncHandler(LinuxWindowBackend *backend) {
    if (!glXGetVideoSyncSGI || !glXWaitVideoSyncSGI) {
        fprintf(stderr, "VSync functions not supported\n");
        return -1;
    }

    unsigned int count;
    glXGetVideoSyncSGI(&count);
    printf("Initial VSync Count: %u\n", count);

    while (1) {
        glXWaitVideoSyncSGI(2, (count + 1) % 2, &count);
        printf("VSync event fired. New Count: %u\n", count);
    }

    return 0;
}
