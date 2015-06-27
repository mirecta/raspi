#!/usr/bin/python

import lcd

ds = lcd.Glcd()
ds.init()
ds.clear()
ds.setBacklight(6)
f = lcd.TTFont("arial.ttf",12)
ds.setFont(f)
m = ds.drawString(0,0,"test",0)
ds.redraw(0,0,m.width,m.height)


ds.drawrect(0,0,50,31,1);
ds.line(0,0,50,31,1);
ds.redraw(0,0,51,31)


