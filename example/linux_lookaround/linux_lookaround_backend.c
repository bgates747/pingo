#include "linux_lookaround_backend.h"
#include "render/renderer.h"
#include "render/image.h"
#include "render/pixel.h"
#include "render/depth.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

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

void init_x() {
    if (dis != 0)
        return;

    dis = XOpenDisplay((char *)0);
    if (dis == 0) {
        fprintf(stderr, "Error: Could not open display\n");
        exit(-1);
    }

    screen = DefaultScreen(dis);
    unsigned long black, white;
    black = BlackPixel(dis, DefaultScreen(dis));
    white = WhitePixel(dis, DefaultScreen(dis));
    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, totalSize.x, totalSize.y, 5, white, black);
    XSelectInput(dis, win, ExposureMask | KeyPressMask);
    XSetStandardProperties(dis, win, "My Window", "HI!", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
    XMapWindow(dis, win);
    gc = XCreateGC(dis, win, 0, 0);
    visual = DefaultVisual(dis, 0);

    if (win == 0 || gc == 0 || visual == 0) {
        fprintf(stderr, "Error: X window initialization failed\n");
        exit(-1);
    }

    printf("X window initialized\n");
}

void init(Renderer *ren, Backend *backend, Vec4i _rect) {
    rect = _rect;
    init_x();
    printf("Backend initialized\n");
}

void beforeRender(Renderer *ren, Backend *backend) {
    LinuxLookaroundBackend *this = (LinuxLookaroundBackend *) backend;
    printf("Before render\n");
}

XImage *create_ximage(Display *display, Visual *visual, int width, int height) {
    printf("Creating XImage\n");
    XImage *ximage = XCreateImage(display, visual, 24, ZPixmap, 0, (char *)framebuffer, width, height, 32, 0);
    if (!ximage) {
        fprintf(stderr, "Error: Could not create XImage\n");
        exit(-1);
    }
    printf("XImage created\n");
    return ximage;
}

void afterRender(Renderer *ren, Backend *backend) {
    printf("After render\n");
    if (!img) {
        printf("Creating XImage in afterRender\n");
        img = create_ximage(dis, visual, totalSize.x, totalSize.y);
    }

    XEvent event;
    XNextEvent(dis, &event);
    {
        XClearArea(dis, win, 0, 0, 1, 1, true);
        XPutImage(dis, win, gc, img, 0, 0, 0, 0, totalSize.x, totalSize.y);
        XFlush(dis);
    }
}

Pixel *getFramebuffer(Renderer *ren, Backend *backend) {
    printf("Getting framebuffer\n");
    if (!framebuffer) {
        fprintf(stderr, "Error: framebuffer is NULL\n");
        exit(-1);
    }
    return framebuffer;
}

PingoDepth *getZetaBuffer(Renderer *ren, Backend *backend) {
    printf("Getting Zeta buffer\n");
    if (!zetaBuffer) {
        fprintf(stderr, "Error: zetaBuffer is NULL\n");
        exit(-1);
    }
    return zetaBuffer;
}

void linuxLookaroundBackendInit(LinuxLookaroundBackend *this, Vec2i size) {
    totalSize = size;
    this->backend.init = &init;
    this->backend.beforeRender = &beforeRender;
    this->backend.afterRender = &afterRender;
    this->backend.getFramebuffer = &getFramebuffer;
    this->backend.getZetaBuffer = &getZetaBuffer;
    this->backend.drawPixel = 0;

    zetaBuffer = malloc(size.x * size.y * sizeof(PingoDepth));
    framebuffer = malloc(size.x * size.y * sizeof(Pixel));

    if (!zetaBuffer || !framebuffer) {
        fprintf(stderr, "Error: Could not allocate memory for buffers\n");
        exit(-1);
    }

    printf("Buffers allocated\n");
}
