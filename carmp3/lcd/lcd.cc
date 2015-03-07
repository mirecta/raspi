// spi.c
//
// Example program for bcm2835 library
// Shows how to interface with SPI to transfer a byte to and from an SPI device
//
// After installing bcm2835, you can build this 
// with something like:
// gcc -o spi spi.c -l bcm2835
// sudo ./spi
// g++ -o lcd lcd.cc -l bcm2835
// Or you can test it before installing with:
// gcc -o spi -I ../../src ../../src/bcm2835.c spi.c
// sudo ./spi
//
// Author: Mike McCauley
// Copyright (C) 2012 Mike McCauley
// $Id: RF22.h,v 1.21 2012/05/30 01:51:25 mikem Exp $

#include <bcm2835.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "font/font_big.c"
#include "font/font_small.c"

#define FONT_SMALL 0
#define FONT_BIG 1

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



class Glcd{
   public:	
	Glcd();
        int init();
        void setFont(int type);
        void clear();
        void putpixel(int x, int y, char c);
        void redraw(int x, int y, int width, int height);
        void fillrect(int x, int y, int width, int height, char c);
        void putchar(int x, int y, const int c);
        int  drawString(int x, int y, const std::string &str, int cut = 0);
        void drawBitmap(int x, int y, int width, int height, const char *bitmap);
        void drawBitmap(int x, int y, int width, int height, const std::string &bitmap);
	int getFontWidth();
	int getFontHeight();
	int getWidth();
	int getHeight();

	virtual ~Glcd();


   private:
        void cmd(uint8_t c);
        void data(uint8_t d);
        void data16(uint16_t d);
        void syncAll();
        void colorOp(uint16_t *data, uint16_t value, char c);
        int16_t grabBitmapRow(int x, int y, int count,  int width, const char *bitmap, int rightAlign = 0);
   private:
        uint16_t *fb;
        const char *font;
        int fontWidth;
	int fontHeight;
	int fontBPC;
        int dwidth;
        int dheight;


};

Glcd::Glcd():fb(0),font(0),dwidth(128),dheight(64){

	fb = new uint16_t[1024];
	setFont(0);        

}

void Glcd::setFont(int type){

	switch(type){

		case FONT_SMALL:
			font = font_small;
			fontWidth = 8;
			fontHeight = 13;
			fontBPC = 13;
			break;

		case FONT_BIG:
			font = font_big;
			fontWidth = 10;
			fontHeight = 20;
			fontBPC = 40;
			break;
	}
}

int Glcd::init(){
      if (!bcm2835_init())
	return 1;

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, HIGH);      // the default
    
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

void Glcd::putpixel(int x, int y, char c){

        if (y > 31){
          y -= 32;
          x+= 128;
	}

	int fbindex = (y << 4) + (x >> 4);
        int bit = 15 - (x % 16) ;
        if(c)
		fb[fbindex] |= (1 << (bit));
        else
		fb[fbindex] &= ~(1 << (bit));

}

void Glcd::clear(){
   memset(fb,0,1024*sizeof(uint16_t));
   syncAll();
}


int  Glcd::drawString(int x, int y, const std::string &str, int cut){

	uint8_t data, byte, stat = 9;
	uint32_t unic = 0;
        int xpos = x;
        int cpos = 0;
	const char *s = str.c_str();        

        for (std::string::const_iterator i = str.begin(); i != str.end(); ++i){
              
                byte = *i;

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
			//printf("U+%04X\n", unic);
			if(xpos < dwidth && cpos >= cut){
				putchar(xpos,y,unic);
				xpos += fontWidth;
			}
                        ++cpos;   
			unic = 0;
		}

		if (stat == 1) {
			// the byte is not allowed here; the state would have to
			// be reset to continue meaningful reading of the string
		}

	}
return cpos;
}



void Glcd::putchar(int x, int y, const int c){
	
	drawBitmap(x,y,fontWidth,fontHeight,font + (c-32)*fontBPC);
}

void Glcd::drawBitmap(int x, int y, int width, int height, const std::string &bitmap){
     drawBitmap(x,y,width,height,bitmap.data());
}


void Glcd::drawBitmap(int x, int y, int width, int height, const char *bitmap){
	int ix = 0;
        int iy = 0;
        int left = x >> 4;
	int right = ((x + width - 1) >> 4);
        int lbit = (16 - (x%16)) ;
        int rbit = (x + width - 1)%16;
        //	      printf("lbit: %d\n",lbit);
	//        printf("left %d, right %d",left,right);
        int smallShift = (width < lbit)? lbit-width:0; 	
        int smallMask = smallShift?(1 << (smallShift))-1:0;
	for (int j = y; j < y + height; ++j,++iy){
                ix = 0;
                if (j > 63) continue;
		if ( j < 32){
			for (int i = left; i <= right; ++i){
                                if(i > 7) continue;
				if(i == left){
					fb[(j << 4) + i ] = (fb[(j << 4) + i ] & ((0xffff << (16 -(x%16)))|(smallMask)))| grabBitmapRow(ix,iy,lbit,width,bitmap,1) << smallShift;
                                        ix += lbit;
				}else if (i == right){
					fb[(j << 4) + i ] = (fb[(j << 4) + i ] & (0xffff >> ((x+width)%16)))| grabBitmapRow(ix,iy,rbit,width,bitmap);
				}else {                       
					fb[(j << 4) + i ] = grabBitmapRow(ix,iy,16,width,bitmap);
                                        ix += 16;  
				}                       
			}
		}else{
			for (int i = left + 8; i <= right + 8; ++i){
                                if(i > 15) continue;
				if(i == left + 8){
					fb[((j - 32) << 4) + i ] = (fb[((j - 32) << 4) + i ] & ((0xffff << (16-(x%16)))|(smallMask)))| grabBitmapRow(ix,iy,lbit,width,bitmap,1) << smallShift;
                                        ix += lbit;
				}else if (i == right + 8){
					fb[((j - 32) << 4) + i ] = (fb[((j - 32) << 4) + i ] & (0xffff >> ((x+width)%16)))| grabBitmapRow(ix,iy,rbit,width,bitmap);
				}else{                        
					fb[((j - 32) << 4) + i ] = grabBitmapRow(ix,iy,16,width,bitmap);
					ix += 16;
				}                         

			}
		}
	}  
}


void Glcd::fillrect(int x, int y, int width, int height, char c){

	int left = x >> 4;
	int right = ((x + width - 1) >> 4);

	uint16_t value = c ? 0xffff:0x0000;
        uint16_t lvalue = 0x00;
        uint16_t rvalue = 0x00;

	if (left == right){
         if (c)
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

int16_t Glcd::grabBitmapRow(int x, int y, int count, int width, const char *bitmap, int rightAlign){

	uint16_t result = 0x0000;
	if (count > 16) return 0x0000;

        if (count > width)
           count = width;
	int index = y*(((width - 1) >> 3) + 1) + (x >> 3);
	int bindex = (x%8);            
//        printf("width:%d y: %d index:%d,bindex:%d, count:%d \n ",width,y,index,bindex,count);
	for( int i = 0; i < count; ++i){
                
                if ( bitmap[index] & (1 << (bindex)))
			result |= (1 << (15-i));

		if(bindex + 1 > 7){
			index ++;
			bindex = 0;
		} else{
			bindex ++;
		}
	}
        if (count < 15 && rightAlign)
	   result = result >> (16 - count);
        
  //      printf("res:%x\n",result&0xffff);
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
  bcm2835_spi_writenb(buffer,3);
  usleep(2);
}

void Glcd::data(uint8_t d){
  // RS 1
  char buffer[] = {0xfa,0x00,0x00};
  buffer[1] = d & 0xf0;
  buffer[2] = d << 4;
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










int main(int argc, char **argv)
{
   Glcd lcd;
   lcd.init();
   lcd.clear();    


//for(int i = 127; i >= 0 ; i-=5){
//lcd.fillrect(0,0,128,20,1);

//lcd.putchar(0,0,237);
lcd.setFont(1);
int i = 0;
int sz= lcd.drawString(0,0,"Mirko je náš malý macko!!!");

while(1){
lcd.fillrect(i,0,128,20,0);
lcd.drawString(0,0,"Mirko je náš malý macko!!!",i%(sz-12));
 ++i;
lcd.redraw(0,0,128,20);
delay(800);
}
//lcd.drawBitmap(atoi(argv[1]),0,10,20,font_10_20);
//lcd.drawBitmap(10,0,10,20,font_10_20);
//lcd.drawBitmap(20,0,10,20,font_10_20);
//lcd.drawBitmap(30,0,10,20,font_10_20);
//printf("i:%d\n",i);
//lcd.drawBitmap(i,0,20,20,b_bits);
//lcd.fillrect(i,0,20,20,0);
//delay(800);
//lcd.putpixel(1,1,1);
//lcd.redraw(0,0,128,20);

//}
return 0;
}






