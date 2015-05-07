
#include "lcd.h"

    
// g++ -std=c++11 lcd.cc -o lcd `freetype-config --cflags` -l bcm2835  `freetype-config --libs`


/*

   g++ -std=c++11 -c lcd.cc -o lcd.o `freetype-config --cflags`
   g++ -std=c++11 -c test.cc -o test.o `freetype-config --cflags`
   g++ -std=c++11 -o lcd  lcd.o test.o -l bcm2835  `freetype-config --libs`
   */








#define mapy_cz_width 128
#define mapy_cz_height 36

static const char mapy_cz_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x1e,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf8, 0x01, 0x1f, 0x60, 0xc0, 0x0f, 0x00, 0x02, 0x00, 0x00, 0x80,
   0x03, 0x80, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x3f, 0xe0, 0xf8, 0xff, 0x00,
   0x03, 0xc0, 0x00, 0xf0, 0x0f, 0xfc, 0x1f, 0x00, 0x00, 0xf8, 0x83, 0x3f,
   0xf0, 0xf8, 0xff, 0x07, 0x03, 0xe0, 0x00, 0xfc, 0x0f, 0xfe, 0x7f, 0x00,
   0x00, 0xf8, 0xc7, 0x3f, 0xf0, 0xf9, 0xff, 0x1f, 0x03, 0xf0, 0x00, 0xfe,
   0x0f, 0xfe, 0x7f, 0x00, 0x00, 0xfc, 0xe7, 0x3f, 0xf8, 0x31, 0x80, 0x3f,
   0x03, 0xf8, 0x00, 0x7f, 0x00, 0xfe, 0xff, 0x00, 0x00, 0xbc, 0xf7, 0x1f,
   0xf8, 0x03, 0x00, 0x3c, 0x03, 0xfe, 0x80, 0x1f, 0x00, 0x00, 0xfe, 0x00,
   0x00, 0x3c, 0xff, 0x1e, 0xbc, 0x07, 0x70, 0x78, 0xff, 0x7f, 0xc0, 0x07,
   0x00, 0x00, 0x7f, 0x00, 0x00, 0x3c, 0x7f, 0x1e, 0x9c, 0x07, 0x70, 0x78,
   0xff, 0x7f, 0xc0, 0x03, 0x00, 0xe0, 0x1f, 0x00, 0x00, 0x3c, 0x3e, 0x1e,
   0x1e, 0x0f, 0xf8, 0x7f, 0xfe, 0x3f, 0xc0, 0x01, 0x00, 0xfc, 0x03, 0x00,
   0x00, 0x1e, 0x1e, 0x1e, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0x1f, 0xc0, 0x01,
   0x00, 0x7f, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0xde, 0xff, 0xff, 0xff, 0x3f,
   0xfc, 0x03, 0xc0, 0x01, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xde,
   0xff, 0xff, 0xff, 0x01, 0x3c, 0x00, 0xc0, 0x01, 0xf0, 0x03, 0x00, 0x00,
   0x00, 0x1f, 0x00, 0xde, 0xff, 0x7f, 0xf0, 0x00, 0x3c, 0x00, 0xc0, 0xff,
   0xff, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0xde, 0xff, 0x3e, 0xf0, 0x00,
   0x3c, 0x00, 0x8f, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x0f, 0x00, 0x9e,
   0x03, 0x38, 0xf0, 0x00, 0x3c, 0x00, 0x8f, 0xff, 0xff, 0xff, 0xff, 0x07,
   0x80, 0x0f, 0x00, 0xdc, 0x01, 0x78, 0xf0, 0x00, 0x3c, 0x00, 0x0f, 0xff,
   0xf7, 0xff, 0xff, 0x01, 0x80, 0x07, 0x00, 0xcc, 0x00, 0x70, 0xf0, 0x00,
   0x38, 0x00, 0x0f, 0xfe, 0xe1, 0xff, 0x3f, 0x00, 0xc0, 0x07, 0x00, 0x88,
   0x00, 0x60, 0x40, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00,
   0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static const  char mapy_cz_bitsi[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfe, 0xe1,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x07, 0xfe, 0xe0, 0x9f, 0x3f, 0xf0, 0xff, 0xfd, 0xff, 0xff, 0x7f,
   0xfc, 0x7f, 0xff, 0xff, 0xff, 0x07, 0xfc, 0xc0, 0x1f, 0x07, 0x00, 0xff,
   0xfc, 0x3f, 0xff, 0x0f, 0xf0, 0x03, 0xe0, 0xff, 0xff, 0x07, 0x7c, 0xc0,
   0x0f, 0x07, 0x00, 0xf8, 0xfc, 0x1f, 0xff, 0x03, 0xf0, 0x01, 0x80, 0xff,
   0xff, 0x07, 0x38, 0xc0, 0x0f, 0x06, 0x00, 0xe0, 0xfc, 0x0f, 0xff, 0x01,
   0xf0, 0x01, 0x80, 0xff, 0xff, 0x03, 0x18, 0xc0, 0x07, 0xce, 0x7f, 0xc0,
   0xfc, 0x07, 0xff, 0x80, 0xff, 0x01, 0x00, 0xff, 0xff, 0x43, 0x08, 0xe0,
   0x07, 0xfc, 0xff, 0xc3, 0xfc, 0x01, 0x7f, 0xe0, 0xff, 0xff, 0x01, 0xff,
   0xff, 0xc3, 0x00, 0xe1, 0x43, 0xf8, 0x8f, 0x87, 0x00, 0x80, 0x3f, 0xf8,
   0xff, 0xff, 0x80, 0xff, 0xff, 0xc3, 0x80, 0xe1, 0x63, 0xf8, 0x8f, 0x87,
   0x00, 0x80, 0x3f, 0xfc, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0xc3, 0xc1, 0xe1,
   0xe1, 0xf0, 0x07, 0x80, 0x01, 0xc0, 0x3f, 0xfe, 0xff, 0x03, 0xfc, 0xff,
   0xff, 0xe1, 0xe1, 0xe1, 0x01, 0x00, 0x00, 0x80, 0x01, 0xe0, 0x3f, 0xfe,
   0xff, 0x80, 0xff, 0xff, 0xff, 0xe1, 0xf3, 0x21, 0x00, 0x00, 0x00, 0xc0,
   0x03, 0xfc, 0x3f, 0xfe, 0x3f, 0xf0, 0xff, 0xff, 0xff, 0xe1, 0xff, 0x21,
   0x00, 0x00, 0x00, 0xfe, 0xc3, 0xff, 0x3f, 0xfe, 0x0f, 0xfc, 0xff, 0xff,
   0xff, 0xe0, 0xff, 0x21, 0x00, 0x80, 0x0f, 0xff, 0xc3, 0xff, 0x3f, 0x00,
   0x00, 0xfe, 0xff, 0xff, 0xff, 0xf0, 0xff, 0x21, 0x00, 0xc1, 0x0f, 0xff,
   0xc3, 0xff, 0x70, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xf0, 0xff, 0x61,
   0xfc, 0xc7, 0x0f, 0xff, 0xc3, 0xff, 0x70, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0x7f, 0xf0, 0xff, 0x23, 0xfe, 0x87, 0x0f, 0xff, 0xc3, 0xff, 0xf0, 0x00,
   0x08, 0x00, 0x00, 0xfe, 0x7f, 0xf8, 0xff, 0x33, 0xff, 0x8f, 0x0f, 0xff,
   0xc7, 0xff, 0xf0, 0x01, 0x1e, 0x00, 0xc0, 0xff, 0x3f, 0xf8, 0xff, 0x77,
   0xff, 0x9f, 0xbf, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0x00, 0xfe, 0xff,
   0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

int main(int argc, char **argv)
{
   Glcd lcd;
   lcd.init();
   lcd.clear();    


//for(int i = 127; i >= 0 ; i-=5){
//lcd.fillrect(0,0,128,20,1);

//lcd.putchar(0,0,237);
//lcd.setFont(1);
int i = 0;
//std::string text(argv[2]);


TTFont font("arial.ttf",25);
lcd.setFont(font);

TextMetrics m = lcd.drawString(0,0,"bla1bla2bla3bla4bla5bla6" , atoi(argv[1]));

lcd.setBacklight(20);
//lcd.drawBitmap(0,0,128,36,std::string(mapy_cz_bits,128*36));
//lcd.redraw(0,0,128,36);

lcd.redraw(0,0,128,m.height);
printf("lost %d \n ",m.lost);

/*while(1){
if (m.lost > 0){
++i;
}else{
i = 0;
}
m = lcd.drawString(0,0,text,i);


delay(800);


}
*/

/*while(1){
lcd.fillrect(i,0,128,20,0);
lcd.drawString(0,0,"Mirko je náš malý macko!!!",i%(sz-12));
 ++i;
lcd.redraw(0,0,128,20);
delay(800);
}*/
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

