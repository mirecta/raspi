#!/usr/bin/python

import rotary
import time

def call(a):
    print a

rotary.RotaryEncoder(6,10,11,call)

while True:
    time.sleep(5000)
