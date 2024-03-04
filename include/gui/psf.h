#ifndef __MYOS__GUI__PSF_H
#define __MYOS__GUI__PSF_H

//#include "gui/widget.h"
//#include "gui/window.h"
#include "common/types.h"

extern "C" char binary_font_psf_end;
extern "C" char binary_font_psf_size;
extern "C" char binary_font_psf_start[];


namespace myos {
    namespace gui {
        #define PSF1_FONT_MAGIC 0x0436
        typedef struct {
            common::uint16_t magic; // Magic bytes for identification.
            common::uint8_t fontMode; // PSF font mode.
            common::uint8_t characterSize; // PSF character size.
        } PSF1_Header;
        
        #define PSF_FONT_MAGIC 0x864ab572
        typedef struct {
            common::uint32_t magic;         /* magic bytes to identify PSF */
            common::uint32_t version;       /* zero */
            common::uint32_t headersize;    /* offset of bitmaps in file, 32 */
            common::uint32_t flags;         /* 0 if there's no unicode table */
            common::uint32_t numglyph;      /* number of glyphs */
            common::uint32_t bytesperglyph; /* size of each glyph */
            common::uint32_t height;        /* height in pixels */
            common::uint32_t width;         /* width in pixels */
        } PSF_font;
    }
}

#endif