#!/usr/bin/python

import lcd

ds = lcd.Glcd()
ds.init()
ds.clear()
f = lcd.TTFont("arial.ttf",13)
ds.setFont(f)
m = ds.drawString(0,0,"Python test1",0)
ds.redraw(0,0,m.width,m.height)

