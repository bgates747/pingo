#include "linux_backend.h"
#include "../../render/renderer.h"
#include "../../render/texture.h"
#include "../../render/pixel.h"
#include "../../render/depth.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

Vec4i rect;
Vec2i totalSize;

PingoDepth *zetaBuffer;
Pixel *frameBuffer;

Display *dis = 0;
int screen;
Window win;
GC gc;
XImage *img = 0;
Visual *visual;

void init_x() {
    if (dis != 0)
        return;
    dis = XOpenDisplay(NULL);
    screen = DefaultScreen(dis);
    unsigned long black = BlackPixel(dis, screen);
    unsigned long white = WhitePixel(dis, screen);
    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, totalSize.x, totalSize.y, 5, white, black);
    XSetStandardProperties(dis, win, "My Window", "HI!", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | KeyPressMask);
    XMapWindow(dis, win);
    gc = XCreateGC(dis, win, 0, 0);
    visual = DefaultVisual(dis, screen);
}

void init(Renderer *ren, BackEnd *backEnd, Vec4i _rect) {
    rect = _rect;
    init_x();
}

void beforeRender(Renderer *ren, BackEnd *backEnd) {
    // Assuming LinuxBackEnd *linuxBackend is intended to be used here
}

XImage *create_ximage(Display *display, Visual *visual, int width, int height) {
    return XCreateImage(display, visual, 24, ZPixmap, 0, (char *)frameBuffer, width, height, 32, 0);
}

void afterRender(Renderer *ren, BackEnd *backEnd) {
    if (!img)
        img = create_ximage(dis, visual, totalSize.x, totalSize.y);
    XEvent event;
    while (XCheckWindowEvent(dis, win, ExposureMask | ButtonPressMask | KeyPressMask, &event)) {
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, totalSize.x, totalSize.y);
    }
    XFlush(dis);
}

Pixel *getFrameBuffer(Renderer *ren, BackEnd *backEnd) {
    return frameBuffer;
}

PingoDepth *getZetaBuffer(Renderer *ren, BackEnd *backEnd) {
    return zetaBuffer;
}

void linuxBackEndInit(LinuxBackEnd *linuxBackend, Vec2i size) {
    totalSize = size;
    linuxBackend->backEnd.init = init;
    linuxBackend->backEnd.beforeRender = beforeRender;
    linuxBackend->backEnd.afterRender = afterRender;
    linuxBackend->backEnd.getFrameBuffer = getFrameBuffer;
    linuxBackend->backEnd.getZetaBuffer = getZetaBuffer;
    linuxBackend->backEnd.drawPixel = NULL;

    zetaBuffer = malloc(size.x * size.y * sizeof(PingoDepth));
    frameBuffer = malloc(size.x * size.y * sizeof(Pixel));
}
