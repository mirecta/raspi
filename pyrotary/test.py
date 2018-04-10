#!/usr/bin/python

import rotary
import time

old = 0
def call(a):
    pass

#rotary.RotaryEncoder(6,10,11,call)
rotary.RotaryEncoder(6,10,11)

while True:
    time.sleep(0.1)
    val = int(rotary.getValue()/2.0)
    if old != val:
        print val
        old = val
