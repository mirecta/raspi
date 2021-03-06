// lcd library


#include "lcd.h"
#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>




//http://dbader.org/blog/monochrome-font-rendering-with-freetype-and-python
//http://www.freetype.org/freetype2/docs/tutorial/step2.html#section-4

#define swap(a, b) { int t = a; a = b; b = t; }

#define PWM_CHANNEL 0


#define ASCII_IN_TABLE 1

static const uint8_t utf8d[] = {

#if ASCII_IN_TABLE
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#endif

  070,070,070,070,070,070,070,070,070,070,070,070,070,070,070,070,
  050,050,050,050,050,050,050,050,050,050,050,050,050,050,050,050,
  030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,
  030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,
  204,204,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
  188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
  174,158,158,158,158,158,158,158,158,158,158,158,158,142,126,126,
  111, 95, 95, 95, 79,207,207,207,207,207,207,207,207,207,207,207,

  0,1,1,1,8,7,6,4,5,4,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
  1,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,8,7,6,4,5,4,3,2,1,1,1,1,

};



static const char bitReverse256[] = 
{
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};


std::vector<int> decodeUTF8( const std::string &str ){

        std::vector<int> result;
        result.reserve(str.size());

	uint8_t data, stat = 9;
	uint32_t unic = 0;

        for (uint8_t byte : str){
              

		// Each byte is associated with a character class and a mask;
		// The character class is used to advance a finite automaton;
		// The mask is used to strip off leading bits from the byte;
		// The remaining bits are combined into a Unicode code point;
		// A code point is complete if the DFA enters the final state.

#if ASCII_IN_TABLE
		data = utf8d[ byte ];
		stat = utf8d[ 256 + (stat << 4) + (data >> 4) ];
		byte = (byte ^ (uint8_t)(data << 4));
#else
		if (byte < 0x80) {
			stat = utf8d[ 128 + (stat << 4) ];
		} else {
			data = utf8d[ byte - 0x80 ];
			stat = utf8d[ 128 + (stat << 4) + (data >> 4) ];
			byte = (byte ^ (uint8_t)(data << 4));
		}
#endif

		unic = (unic << 6) | byte;

		if (!stat) {
			// unic is now a proper code point, we just print it out.
			result.push_back(unic);
			unic = 0;
		}

		if (stat == 1) {
			// the byte is not allowed here; the state would have to
			// be reset to continue meaningful reading of the string
		}

	}
return result;
}


Glyph::Glyph():bitmap(0){}

Glyph::Glyph(const Glyph &g):
    height(g.height),width(g.width),pitch(g.pitch),
    left(g.left),top(g.top),
    advance_width(g.advance_width),ascent(g.ascent),
    descent(g.descent),bitmap(g.bitmap){
        //take ownership
        g.release();
    }

void Glyph::release() const{
    bitmap = 0;
}

Glyph::Glyph(const FT_GlyphSlot slot):bitmap(0){
    set(slot);
}

void Glyph::set(const FT_GlyphSlot slot){

    width = slot->bitmap.width;
    height = slot->bitmap.rows;
    pitch = slot->bitmap.pitch;
    top = slot->bitmap_top;
    left = slot->bitmap_left;
    advance_width = slot->advance.x >> 6;
    descent = std::max(0, height - top);
    ascent = std::max(0, std::max(top, height) - descent);

    if (bitmap)
        delete [] bitmap;

    bitmap = new char [height*abs(pitch)];

    //copy reverse

    for (int i = 0; i < height*abs(pitch);++i){

        bitmap[i] = bitReverse256[slot->bitmap.buffer[i]];
    }

    //std::copy(slot->bitmap.buffer, slot->bitmap.buffer + height*abs(pitch), bitmap);
}
Glyph::~Glyph(){
    if (bitmap)
        delete [] bitmap;
}


FT_Library TTFont::library = 0;

TTFont::TTFont(const std::string &fontfile, int pixelSize)
          :pixelSize(pixelSize){
          
    int error;
    if (library == 0){
        error = FT_Init_FreeType( &library );
        if(error) printf("error init library \n");
    }

    error = FT_New_Face( library, fontfile.c_str(),0,&face );
    if(error) printf("error load fonface \n");          
    
    error = FT_Set_Pixel_Sizes(face, 0, pixelSize );
    if(error) printf("error to set pixel size \n");
}
  
TTFont::~TTFont(){
   if (face)
    FT_Done_Face(face);
}
	

TextMetrics TTFont::getTextSize(const std::vector<int> &text){

        int width = 0;
        int max_ascent = 0;
        int max_descent = 0;

        FT_UInt  glyph_index, previous = 0;
        FT_Bool  use_kerning;

        use_kerning = FT_HAS_KERNING( face );

        for(auto i : text){
            
            const Glyph *gl = getGlyph(i);
           
            if (use_kerning){
                glyph_index = FT_Get_Char_Index( face, i );
                if (previous && glyph_index)
                {
                    FT_Vector  delta;
                    FT_Get_Kerning( face, previous, glyph_index,
                            FT_KERNING_DEFAULT, &delta );

                    width += delta.x >> 6;
                }
                previous = glyph_index;
            }
            width += gl->advance_width;
           max_ascent = std::max(max_ascent, gl->ascent);
           max_descent = std::max(max_descent, gl->descent);

		    
        }
        return TextMetrics(width,max_ascent + max_descent,max_descent);
}



const Glyph* TTFont::getGlyph(int code){

    //try to find in map 
    auto it = glyphs.find(code);
    if (it != glyphs.end())
        return &it->second;
    

    int error = FT_Load_Char( face, code, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO );
    if(error) printf("error to render char \n");

    //if no insert it 
    it = glyphs.insert(std::make_pair(code,Glyph(face->glyph))).first;
    
    return &it->second;
}








Glcd::Glcd():fb(0),dwidth(128),dheight(64),font(0){

	fb = new uint16_t[1024];

}

void Glcd::setFont(TTFont &fnt){
	font = &fnt;
}

int Glcd::init(){
      if (!bcm2835_init())
	return 1;

    //spi
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, HIGH);      // the default

    //pwm
    bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_12, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);

    bcm2835_pwm_set_range(PWM_CHANNEL, 16);
    //bcm2835_pwm_set_data(PWM_CHANNEL, 1023);
 
    cmd(0x30);
    cmd(0x30);
    cmd(0x0c);
    cmd(0x06);

    cmd(0x01); //clear
    usleep(20000);
    cmd(0x34);    
    cmd(0x36);    

return 0;
}

void Glcd::setBacklight(int value){
	bcm2835_pwm_set_data(PWM_CHANNEL,value);
}

void Glcd::putpixel(int x, int y, int c){
       
        if(x >= dwidth || y >= dheight) 
	   return;    
   
        if (y > 31){
          y -= 32;
          x += 128;
	}

	int fbindex = (y << 4) + (x >> 4);
        int bit = 15 - (x % 16) ;
        if(c)
		fb[fbindex] |= (1 << (bit));
        else
		fb[fbindex] &= ~(1 << (bit));

}

void Glcd::line(int x0, int y0, int x1, int y1, int c){



	int steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int err = dx / 2;
	int ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			putpixel(y0, x0, c);
		} else {
			putpixel(x0, y0, c);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void Glcd::lineH(int x, int y, int width, int c){
       
        if(x >= dwidth || y >= dheight)
	  return;

        if((x + width) > dwidth)
            width = dwidth - x;
 
	fillRect(x,y,width,1,c);

}

void Glcd::lineV(int x, int y, int height, int c){

        if(x >= dwidth || y >= dheight)
	  return;

        if((y + height) > dheight)
            height = dheight - y;
	
	fillRect(x,y,1,height,c);
}


void Glcd::drawRect(int x, int y, int width, int height, int c){

	lineH(x, y, width, c);
	lineH(x, y+height-1, width, c);
	lineV(x, y, height, c);
	lineV(x+width-1, y, height, c);

}



void Glcd::clear(){
   memset(fb,0,1024*sizeof(uint16_t));
   syncAll();
}


TextMetrics  Glcd::drawString(int x, int y, const std::string &str, int cut){
 	int doCut = 0;	

	if(font == 0 )
		return TextMetrics();

	std::vector<int> unicode(decodeUTF8(str));
	TextMetrics metrics = font->getTextSize(unicode);
        metrics.count = unicode.size();	

        //clear under text
	fillRect(x,y,metrics.width,metrics.height,0);

	FT_UInt  glyph_index, previous = 0;
	FT_Bool  use_kerning;

	use_kerning = FT_HAS_KERNING( font->face );

	for(auto i : unicode){
		
		if(doCut < cut){
			++doCut;
			continue;
		}
		const Glyph *gl = font->getGlyph(i);

		if (use_kerning){
			glyph_index = FT_Get_Char_Index( font->face, i );
			if (previous && glyph_index)
			{
				FT_Vector  delta;
				FT_Get_Kerning( font->face, previous, glyph_index,
						FT_KERNING_DEFAULT, &delta );

				x += delta.x >> 6;
			}
			previous = glyph_index;
		}
		if((x + gl->left) < dwidth && (y + gl->top) < dheight){
			drawBitmap(x + gl->left, y + metrics.height - gl->ascent - metrics.baseline, gl->width, gl->height, gl->pitch, gl->bitmap, 1);
                        //printf("glyph Y %d\n",y + metrics.height - gl->ascent - metrics.baseline);
		}else if ((x + gl->left) > dwidth){
			metrics.lost++;
                }
		
		x += gl->advance_width;

	}

	return metrics;
}




void Glcd::drawBitmap(int x, int y, int width, int height, const std::string &bitmap){
      int pitch = ((width - 1) >> 3) + 1;
      drawBitmap(x,y,width,height,pitch,bitmap.data());
}

void Glcd::drawBitmap(int x, int y, int width, int height, int pitch, const std::string &bitmap){
      drawBitmap(x,y,width,height,pitch,bitmap.data());
}


void Glcd::drawBitmap(int x, int y, int width, int height, int pitch, const char *bitmap, int nomask){
	int ix = 0;
        int iy = 0;
        int left = x >> 4;
	int right = ((x + width - 1) >> 4);
        int lbit = (16 - (x%16)) ;
        int rbit = (x + width - 1)%16 + 1;
	//printf("x:%d, y:%d \n",x,y);
        //printf("lbit: %d , rbit: %d ",lbit,rbit);
	//printf("left %d, right %d \n",left,right);
        int smallShift = (width < lbit) ? lbit-width:0; 	
        int smallMask = smallShift ? (1 << (smallShift))-1:0;
        
        int leftMask = nomask ? 0xffff : ((0xffff << (16 -(x%16)))|(smallMask));
        int rightMask = nomask ? 0xffff : (0xffff >> ((x+width)%16));

	for (int j = y; j < y + height; ++j,++iy){
                ix = 0;
                if (j > 63) continue;
                //printf("index %d, left %d \n",(j << 4),left );
		if ( j < 32){
			for (int i = left; i <= right; ++i){
                                if(i > 7) continue;
				if(i == left){
					fb[(j << 4) + i ] = (fb[(j << 4) + i ] & leftMask)| grabBitmapRow(ix,iy,lbit,width,pitch,bitmap,1) << smallShift;
                                        ix += lbit;
				}else if (i == right){
					fb[(j << 4) + i ] = (fb[(j << 4) + i ] & rightMask)| grabBitmapRow(ix,iy,rbit,width,pitch,bitmap);
				}else {                       
					if(nomask)
						fb[(j << 4) + i ] |= grabBitmapRow(ix,iy,16,width,pitch,bitmap);
					else
						fb[(j << 4) + i ] = grabBitmapRow(ix,iy,16,width,pitch,bitmap);
						
                                        ix += 16;  
				}                       
			}
		}else{
			for (int i = left + 8; i <= right + 8; ++i){
                                if(i > 15) continue;
				if(i == left + 8){
					fb[((j - 32) << 4) + i ] = (fb[((j - 32) << 4) + i ] & leftMask)| grabBitmapRow(ix,iy,lbit,width,pitch,bitmap,1) << smallShift;
                                        ix += lbit;
				}else if (i == right + 8){
					fb[((j - 32) << 4) + i ] = (fb[((j - 32) << 4) + i ] & rightMask)| grabBitmapRow(ix,iy,rbit,width,pitch,bitmap);
				}else{  
					if(nomask)                      
						fb[((j - 32) << 4) + i ] |= grabBitmapRow(ix,iy,16,width,pitch,bitmap);
					else
						fb[((j - 32) << 4) + i ] = grabBitmapRow(ix,iy,16,width,pitch,bitmap);
		
					ix += 16;
				}                         

			}
		}
	}  
}


void Glcd::fillRect(int x, int y, int width, int height, int c){

        if(x >= dwidth || y >= dheight)
	  return;
        
        if((x + width) > dwidth)
            width = dwidth - x;
        
        if((y + height) > dheight)
            height = dheight - y;
	

        int left = x >> 4;
	int right = ((x + width - 1) >> 4);

	uint16_t value = c ? 0xffff:0x0000;
        uint16_t lvalue = 0x00;
        uint16_t rvalue = 0x00;

	if (left == right){
    //     if (c)
         lvalue = rvalue = (0xffff >> (x%16)) & 0xffff << (15 - ((x + width-1)%16) );
	}else{
         lvalue = 0xffff >> (x%16);
         rvalue = 0xffff << (15 - ((x + width-1)%16));
	}	

	//        printf("left %d, right %d",left,right);
	for (int j = y; j < y + height; ++j){
                
		if (j > 63) continue;
		if ( j < 32){

			for (int i = left; i <= right; ++i){
                                if(i > 7) continue;
				if(i == left)
					colorOp(&fb[(j << 4) + i ],lvalue,c);
                                else if (i == right)
                                        colorOp(&fb[(j << 4) + i ],rvalue,c); 
				else                        
					fb[(j << 4) + i ] = value;                         
			}
		}else{
			for (int i = left + 8; i <= right + 8; ++i){
                                if(i > 15) continue;
                                if(i == left + 8)
					colorOp(&fb[((j - 32) << 4) + i ],lvalue,c);
                                else if (i == right + 8)
                                        colorOp(&fb[((j - 32) << 4) + i ],rvalue,c); 
				else                        
                       		fb[((j - 32) << 4) + i ] = value;                         
			}
		}
	}  
}


void Glcd::redraw(int x, int y, int width, int height){
        
        if(x >= dwidth || y >= dheight)
	  return;
        
        if((x + width) > dwidth)
            width = dwidth - x;
        
        if((y + height) > dheight)
            height = dheight - y;
	
        int left = x >> 4;
	int right = ((x + width - 1) >> 4);

//        printf("left %d, right %d",left,right);
	for (int j = y; j < y + height; ++j){

		if ( j < 32){
			cmd(0x80|j);
			cmd(0x80|left); 
			for (int i = left; i <= right; ++i){
				data16(fb[(j << 4) + i ]);                         
			}
		}else{
			cmd(0x80|(j - 32));
			cmd(0x80|(left + 8)); 

			for (int i = left + 8; i <= right + 8; ++i){
				data16(fb[((j - 32) << 4) + i ]);                         

			}
		}
	}  
}

int Glcd::getWidth(){
    return dwidth;
}

int Glcd::getHeight(){
    return dheight;
}

int16_t Glcd::grabBitmapRow(int x, int y, int count, int width, int pitch, const char *bitmap, int rightAlign){

	uint16_t result = 0x0000;
	if (count > 16) return 0x0000;

        if (count > width)
           count = width;
	int index = y * pitch + (x >> 3);
	int bindex = (x%8);            
        //printf("width:%d y: %d index:%d,bindex:%d, count:%d \n ",width,y,index,bindex,count);
        //printf("%d -> %02x \n", index ,bitmap[index]);
	for( int i = 0; i < count; ++i){
                
		if ( bitmap[index] & (1 << (bindex)))
			result |= (1 << (15-i));

		if(bindex + 1 > 7){
			index ++;
			bindex = 0;
	//		printf("%d -> %02x \n", index ,bitmap[index]);

		} else{
			bindex ++;
		}
	}
        if (count < 16 && rightAlign)
	   result = result >> (16 - count);
        
        //printf("res:%x\n",result&0xffff);
	return result ;
}

void Glcd::colorOp(uint16_t *data, uint16_t value, char c){

    if(c)
      *data |= value;
    else
      *data &= ~value;

}


void Glcd::syncAll(){
   int index = 0;
   for (int j = 0; j <  32; ++j){
       cmd(0x80|j);
       cmd(0x80);
       for(int i = 0; i < 16; ++i){
        data16(fb[index++]);
       }
   }
}


void Glcd::cmd(uint8_t c){
  // RS 0
  char buffer[] = {0xf8,0x00,0x00};
  buffer[1] = c & 0xf0;
  buffer[2] = c << 4;
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_writenb(buffer,3);
  usleep(2);
}

void Glcd::data(uint8_t d){
  // RS 1
  char buffer[] = {0xfa,0x00,0x00};
  buffer[1] = d & 0xf0;
  buffer[2] = d << 4;
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_writenb(buffer,3);
  usleep(2);
}

void Glcd::data16(uint16_t d){

	data(d >> 8);
	data(d & 0x00ff);
}




Glcd::~Glcd(){

    delete [] fb;
        
    bcm2835_spi_end();
    bcm2835_close();

}










