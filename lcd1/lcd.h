
#include <ft2build.h>
#include <string>
#include <vector>
#include <map>
#include <inttypes.h>
    

#include FT_FREETYPE_H

#ifndef __LCD_H__



#define __LCD_H__

struct TextMetrics {
     TextMetrics()
         :width(0),height(0),baseline(0),lost(0){}
 
     TextMetrics(int w, int h, int bl):width(w),height(h),baseline(bl),lost(0){}

     int width;
     int height;
     int baseline;
     int lost;
};

struct Glyph {

    Glyph();
    Glyph(const Glyph &g);
    Glyph(const FT_GlyphSlot slot);

    void release() const;
    void set(const FT_GlyphSlot slot);

    ~Glyph();
    
    int height;
    int width;
    int pitch;
    int left;
    int top;
    int advance_width;
    int ascent;
    int descent;
    mutable char *bitmap; 
};

class TTFont {
    friend class Glcd;
    public:
        TTFont(const std::string &fontfile, int pixelSize);
        ~TTFont();

        const Glyph* getGlyph(int code);        
        TextMetrics getTextSize(const std::vector<int> &text);

    private:
        TTFont();

    protected:
        FT_Face face;
	
    private:
        static FT_Library  library;
        int pixelSize;
        std::map<int,Glyph> glyphs;
};

class Glcd{
   public:	
	Glcd();
        int init();
        void setFont(TTFont &font);
        void clear();
        void putpixel(int x, int y, char c);
        void redraw(int x, int y, int width, int height);
        void fillrect(int x, int y, int width, int height, char c);
        void putchar(int x, int y, const int c);

        TextMetrics drawString(int x, int y, const std::string &str, int cut = 0);

	void drawBitmap(int x, int y, int width, int height, int pitch, const char *bitmap, int nomask = 0);
        void drawBitmap(int x, int y, int width, int height, const std::string &bitmap);
        void drawBitmap(int x, int y, int width, int height, int pitch, const std::string &bitmap);

	void setBacklight(int value);
	int getWidth();
	int getHeight();
        
	virtual ~Glcd();


   private:
        void cmd(uint8_t c);
        void data(uint8_t d);
        void data16(uint16_t d);
        void syncAll();
        void colorOp(uint16_t *data, uint16_t value, char c);
        int16_t grabBitmapRow(int x, int y, int count,  int width, int pitch,  const char *bitmap, int rightAlign = 0);
   private:
        uint16_t *fb;
        int dwidth;
        int dheight;
        TTFont *font;


};

#endif // __LCD_H__
