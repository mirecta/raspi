#!/usr/bin/python

from lib.RPi_I2C_LCD import LCD
import rotary
import time


def main():
    lcd = LCD()
    lcd.set_backlight(True)
    
    rotary.RotaryEncoder(6,10,11)
    old = 0    
    
    while True:
        butt = rotary.getButton(True)
        if butt: rotary.setValue(0)
        val = int(rotary.getValue()/2.)
        
        if old != val:
            old = val
            lcd.set_cursor(row=0)
            lcd.message("%d    "%val)
        
        time.sleep(0.1)   
 
if __name__ == "__main__":
    main()     
