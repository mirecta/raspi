#!/usr/bin/python

import lcd

ds = lcd.Glcd()
ds.init()
ds.clear()
ds.setBacklight(6)
f = lcd.TTFont("arial.ttf",35)
ds.setFont(f)
m = ds.drawString(0,0,"Tt - test",0)
ds.redraw(0,0,m.width,m.height)

