#!/usr/bin/python

from lib.RPi_I2C_LCD import LCD
import rotary
import time

lcd = LCD()

val = 0


def call(a):
    global val
    val += a/2.0
    lcd.set_cursor(row=0)
    lcd.message("%d    "%int(val))
    #print val




def main():
    
    lcd.set_backlight(True)
    rotary.RotaryEncoder(6,10,11,call)

    # first banner
    lcd.set_cursor(row=0)
    lcd.message("%d    "%val)
    while True:
        time.sleep(1000)   
 
if __name__ == "__main__":
    main()     
