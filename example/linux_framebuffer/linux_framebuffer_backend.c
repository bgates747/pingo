#include "linux_framebuffer_backend.h"

#include "render/depth.h"
#include "render/pixel.h"
#include "render/renderer.h"
#include "render/texture.h"

#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/mman.h> 
#include <sys/ioctl.h> 
#include <fcntl.h> 
#include <linux/fb.h> 
#include <unistd.h> 
#include <stdio.h> 

Vec4i rect;
Vec2i totalSize;
PingoDepth * zetaBuffer;
Pixel * frameBuffer;

void init( Renderer * ren, Backend * backend, Vec4i _rect) {
    rect = _rect;

}

void beforeRender( Renderer * ren, Backend * backend) {
    LinuxFramebufferBackend * this = (LinuxFramebufferBackend *) backend;
}

void afterRender( Renderer * ren,  Backend * backend) {

}

Pixel * getFrameBuffer( Renderer * ren,  Backend * backend) {
    return frameBuffer;
}

PingoDepth * getZetaBuffer( Renderer * ren,  Backend * backend) {
    return zetaBuffer;
}

void linuxFramebufferBackendInit( LinuxFramebufferBackend * this, Vec2i size, const char * framebufferDevice) {
    totalSize = size;
    this->backend.init = &init;
    this->backend.beforeRender = &beforeRender;
    this->backend.afterRender = &afterRender;
    this->backend.getFrameBuffer = &getFrameBuffer;
    this->backend.getZetaBuffer = &getZetaBuffer;
    this->backend.drawPixel = 0;

    zetaBuffer = malloc(size.x*size.y*sizeof (PingoDepth));
    int fdScreen = open( framebufferDevice, O_RDWR );
    frameBuffer = mmap( 0, size.x*size.y*4, PROT_READ | PROT_WRITE, MAP_SHARED, fdScreen, 0 );    
}

