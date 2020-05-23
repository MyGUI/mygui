
#pragma once

#include <cstdlib>
#include "../core/Shape.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace msdfgen {

typedef unsigned unicode_t;

class FreetypeHandle;

/// Global metrics of a typeface (in font units).
struct FontMetrics {
    /// The size of one EM.
    double emSize;
    /// The vertical position of the ascender and descender relative to the baseline.
    double ascenderY, descenderY;
    /// The vertical difference between consecutive baselines.
    double lineHeight;
    /// The vertical position and thickness of the underline.
    double underlineY, underlineThickness;
};

// MyGUI modification: class exposed to use preloaded custom face
class FontHandle {
public:
	friend FontHandle * loadFont(FreetypeHandle *library, const char *filename);
	friend void destroyFont(FontHandle *font);
	friend bool getFontMetrics(FontMetrics &metrics, FontHandle *font);
	friend bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font);
	friend bool loadGlyph(Shape &output, FontHandle *font, unicode_t unicode, double *advance);
	friend bool getKerning(double &output, FontHandle *font, unicode_t unicode1, unicode_t unicode2);

	FT_Face face;
};

/// Initializes the FreeType library.
FreetypeHandle * initializeFreetype();
/// Deinitializes the FreeType library.
void deinitializeFreetype(FreetypeHandle *library);
/// Loads a font file and returns its handle.
FontHandle * loadFont(FreetypeHandle *library, const char *filename);
/// Unloads a font file.
void destroyFont(FontHandle *font);
/// Outputs the metrics of a font file.
bool getFontMetrics(FontMetrics &metrics, FontHandle *font);
/// Outputs the width of the space and tab characters.
bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font);
/// Loads the geometry of a glyph from a font file.
bool loadGlyph(Shape &output, FontHandle *font, unicode_t unicode, double *advance = NULL);
/// Outputs the kerning distance adjustment between two specific glyphs.
bool getKerning(double &output, FontHandle *font, unicode_t unicode1, unicode_t unicode2);

}
