#include "drivers/svga.h"
#include "maths.h"

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::gui;
using namespace myos;

SuperVideoGraphicsArray::SuperVideoGraphicsArray(uint8_t* screen, uint32_t width, uint32_t height, uint32_t pitch, uint8_t depth, uint32_t pixelwidth) 
                        : screen(screen),
                          width(width),
                          height(height),
                          pitch(pitch),
                          depth(depth),
                          pixelwidth(pixelwidth) { 
    BackBuffer = (uint8_t*)(new uint8_t [width * height * pixelwidth]);
    PsfInit();
}

SuperVideoGraphicsArray::~SuperVideoGraphicsArray() {}

void SuperVideoGraphicsArray::PsfInit() {
    uint16_t glyph = 0;
    /* cast the address to PSF header struct */
    PSF_font *font = (PSF_font*)&binary_font_psf_start;
    /* is there a unicode table? */
    if (font->flags) {
        unicode = nullptr;
        return; 
    }
 
    /* get the offset of the table */
    char *s = (char*)(
    (uint8_t*)&binary_font_psf_start +
      font->headersize +
      font->numglyph * font->bytesperglyph
    );
    /* allocate memory for translation table */
    unicode = (uint16_t*)(new uint8_t* [65535 * 2]);
    while((int)s > binary_font_psf_end) {
        uint16_t uc = (uint16_t)(s[0]);
        if(uc == 0xFF) {
            glyph++;
            s++;
            continue;
        } else if(uc & 128) {
            /* UTF-8 to unicode */
            if((uc & 32) == 0 ) {
                uc = ((s[0] & 0x1F)<<6)+(s[1] & 0x3F);
                s++;
            } else
            if((uc & 16) == 0 ) {
                uc = ((((s[0] & 0xF)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F);
                s+=2;
            } else
            if((uc & 8) == 0 ) {
                uc = ((((((s[0] & 0x7)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F))<<6)+(s[3] & 0x3F);
                s+=3;
            } else
                uc = 0;
        }
        /* save translation */
        unicode[uc] = glyph;
        s++;
    }
}

/*uint8_t* SuperVideoGraphicsArray::GetFrameBufferSegment() {
    return 0;
}*/

void SuperVideoGraphicsArray::Redraw() {
    memcpy(screen, BackBuffer, width * height * pixelwidth);
}

void SuperVideoGraphicsArray::PutPixel(uint32_t x, uint32_t y, uint32_t color) {
    uint32_t where = x*pixelwidth + y*pitch;
    BackBuffer[where] = color & 255;              // RED
    BackBuffer[where + 1] = (color >> 8) & 255;   // GREEN
    BackBuffer[where + 2] = (color >> 16) & 255;  // BLUE
    BackBuffer[where + 3] = (color >> 24) & 255;  // BLUE
}

void SuperVideoGraphicsArray::PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t where = x*pixelwidth + y*pitch;
    BackBuffer[where] = b;        // BLUE
    BackBuffer[where + 1] = g;   // GREEN
    BackBuffer[where + 2] = r;  // RED
}

void SuperVideoGraphicsArray::FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    uint8_t* where = BackBuffer;
    where += pitch * y;

    for (uint32_t Y = y; Y < y + h;Y++) {
        for (uint32_t X = x; X < x + w;X++) {
            where[X*pixelwidth] = color & 255;              // BLUE
            where[X*pixelwidth + 1] = (color >> 8) & 255;   // GREEN
            where[X*pixelwidth + 2]= (color >> 16) & 255;  // RED
        }
        where += pitch;
    }
}

void SuperVideoGraphicsArray::FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t* where = BackBuffer;
    where += pitch * y;

    for (uint32_t Y = y; Y < y + h;Y++) {
        for (uint32_t X = x; X < x + w;X++) {
            where[X*pixelwidth] = b;        // BLUE
            where[X*pixelwidth + 1] = g;   // GREEN
            where[X*pixelwidth + 2] = r;  // RED
        }
        where += pitch;
    }
}

void SuperVideoGraphicsArray::PutLine(int x0, int y0, int x1, int y1) {
    uint8_t* where = BackBuffer;
    where += pitch * y0 + x0*pixelwidth;

    if (x0 == x1) {
        for(; y0 < y1; y0++) {
            where[0] = 0x00;        // BLUE
            where[1] = 0x00;   // GREEN
            where[2] = 0x00;  // RED

            where += pitch;
        }
        return;
    }
    if (y0 == y1) {
        for(; x0 < x1; x0++) {
            where[0] = 0x00;        // BLUE
            where[1] = 0x00;   // GREEN
            where[2] = 0x00;  // RED

            where += pixelwidth;
        }
        return;
    }

    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
    int err = dx+dy, e2; /* error value e_xy */
 
    for(;;){  /* loop */
        where[0] = 0x00;        // BLUE
        where[1] = 0x00;   // GREEN
        where[2] = 0x00;  // RED
        //this->PutPixel(x0,y0, 0xFFFFFF);
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 >= dy) { 
            err += dy;  
            x0 += sx; 
            where += sy * pixelwidth;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx) { 
            err += dx; 
            y0 += sy; 
            where += sy * pitch;
        } /* e_xy+e_y < 0 */
    }
}

void SuperVideoGraphicsArray::PutChar(
    /* note that this is int, not char as it's a unicode character */
    uint8_t c,
    /* cursor position on screen, in characters not in pixels */
    uint32_t cx, uint32_t cy,
    /* foreground and background colors, say 0xFFFFFF and 0x000000 */
    uint32_t fg, uint32_t bg)
{
    /* cast the address to PSF header struct */
    PSF_font *font = (PSF_font*)&binary_font_psf_start;
    /* we need to know how many bytes encode one row */
    int bytesperline=(font->width+7)/8;
    /* unicode translation */
    if(unicode != nullptr) {
        c = unicode[c];
    }
    /* get the glyph for the character. If there's no
       glyph for a given character, we'll display the first glyph. */
    uint8_t *glyph =
     (uint8_t*)&binary_font_psf_start +
     font->headersize +
     (c>0&&c<font->numglyph?c:0)*font->bytesperglyph;
    /* calculate the upper left corner on screen where we want to display.
       we only do this once, and adjust the offset later. This is faster. */
    //int offs =
    //    (cy * font->height * pitch) +
    //    (cx * (font->width) * sizeof(uint32_t));
    int offs =
        (cy * pitch) +
        (cx * sizeof(uint32_t));
    /* finally display pixels according to the bitmap */
    int x,y, line;
    //int mask;
    for(y=0;y < font->height;y++){
        /* save the starting position of the line */
        line=offs;
        //mask=1<<(font->width-1);
        /* display a row */
        for(x=0;x < font->width;x++){
            *((uint32_t*)(BackBuffer + line - 1)) = glyph[x/8] & (0x80 >> (x & 7)) ? fg : bg;
            //*((uint32_t*)(BackBuffer + line - 1)) = fg;//???????
            /* adjust to the next pixel */
            //mask >>= 1;
            line += pixelwidth;
            //if(x >= 16) {
            //    break;
            //}
        }
        /* adjust to the next line */
        glyph += bytesperline;
        offs  += pitch;
    }
}

void SuperVideoGraphicsArray::PutString(common::uint8_t* c, common::uint32_t cx, common::uint32_t cy, 
                                        common::uint32_t fg, common::uint32_t bg, common::uint16_t w) {
    int ax = cx;
    //int ay = cy;
    for(int i = 0;(i <= 65535) && (c[i] != '\0'); i++) {
        if(c[i] == '\n') {
            cy += 26 + 1;
            cx = ax;
            continue;
        }
        if(cx - ax >= w) {
            cy += 18;
            cx = ax;
        }
        PutChar(c[i], cx, cy, fg, bg);

        cx += 8 + 1;
    }
}