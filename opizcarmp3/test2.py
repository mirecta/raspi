#!/usr/bin/python

from lib.RPi_I2C_LCD import LCD

lcd = LCD()

def main():
    
    lcd.set_backlight(True)

    # first banner
    lcd.set_cursor(row=0)
    lcd.message("test_test_test.mp3")

if __name__ == "__main__":
    main()     
