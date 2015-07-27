#!/usr/bin/python

import lcd

ds = lcd.Glcd()
ds.init()
ds.clear()
ds.setBacklight(7)
f = lcd.TTFont("DroidSans.ttf",21)
f1 = lcd.TTFont("DroidSans.ttf",15)
ds.setFont(f)
m = ds.drawString(0,0,"Vocal Trance Top 10 (April 2015) / New Trance Mix / Paradise",0)
ds.redraw(0,0,m.width,m.height)


ds.setFont(f1)
m = ds.drawString(0,31,"Vocal Trance Top 10 (April 2015) / New Trance Mix / Paradise",0)
ds.redraw(0,31,m.width,m.height)



#ds.drawRect(0,0,50,31,1);
#ds.line(0,0,50,31,1);
#ds.redraw(0,0,51,31)


