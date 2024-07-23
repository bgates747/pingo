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


Vec4i rect;
Vec2i totalSize;

PingoDepth * zetaBuffer;
Pixel * framebuffer;

Display *dis = 0;
int screen;
Window win;
GC gc;
XImage * img = 0;
Visual *visual;

void init_x() {
    if (dis != 0)
        return;
    dis=XOpenDisplay((char *)0);
    screen=DefaultScreen(dis);
    unsigned long black, white;
    black = BlackPixel(dis, DefaultScreen(dis));
    white = BlackPixel(dis, DefaultScreen(dis));
    win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0, totalSize.x, totalSize.y, 5, white, black);
    XSelectInput(dis, win, ExposureMask | KeyPressMask);
    XSetStandardProperties(dis,win,"My Window","HI!",None,NULL,0,NULL);
    XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
    XMapWindow(dis, win);
    gc=XCreateGC(dis, win, 0,0);
    visual = DefaultVisual(dis, 0);
};

void init( Renderer * ren, Backend * backend, Vec4i _rect) {
    rect = _rect;
    init_x();
}

void beforeRender( Renderer * ren, Backend * backend) {
    LinuxWindowBackend * this = (LinuxWindowBackend *) backend;

}

XImage *create_ximage(Display *display, Visual *visual, int width, int height)
{
    return XCreateImage(display, visual, 24, ZPixmap, 0, (char*)&framebuffer[0], width, height, 32, 0);
}

void afterRender( Renderer * ren,  Backend * backend) {
    if (!img)
    img = create_ximage(dis, visual, totalSize.x, totalSize.y);

    XEvent event;
    XNextEvent(dis, &event);
    //if (event.type == Expose)
    {
        XClearArea(dis, win, 0, 0, 1, 1, true);
        XPutImage(dis, win, gc, img, 0,0,0,0,totalSize.x, totalSize.y );
        //XSync(dis, 1);
        XFlush(dis);
    }

}

Pixel * getFramebuffer( Renderer * ren,  Backend * backend) {
    return framebuffer;
}

PingoDepth * getZetaBuffer( Renderer * ren,  Backend * backend) {
    return zetaBuffer;
}

void linuxWindowBackendInit( LinuxWindowBackend * this, Vec2i size) {
    totalSize = size;
    this->backend.init = &init;
    this->backend.beforeRender = &beforeRender;
    this->backend.afterRender = &afterRender;
    this->backend.getFramebuffer = &getFramebuffer;
    this->backend.getZetaBuffer = &getZetaBuffer;
    this->backend.drawPixel = 0;

    zetaBuffer = malloc(size.x*size.y*sizeof (PingoDepth));
    framebuffer = malloc(size.x*size.y*sizeof (Pixel));
    
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
    PFNGLXGETVIDEOSYNCSGIPROC glXGetVideoSyncSGI = (PFNGLXGETVIDEOSYNCSGIPROC)glXGetProcAddressARB((const GLubyte *)"glXGetVideoSyncSGI");
    PFNGLXWAITVIDEOSYNCSGIPROC glXWaitVideoSyncSGI = (PFNGLXWAITVIDEOSYNCSGIPROC)glXGetProcAddressARB((const GLubyte *)"glXWaitVideoSyncSGI");

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
        // Here you can trigger any event you want when vsync fires
    }

    return 0;
}