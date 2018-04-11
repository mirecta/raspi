#!/usr/bin/python

from lib.RPi_I2C_LCD import LCD
import rotary
import time
import unicodedata
import os
from os.path import join, getsize
import locale

locale.setlocale(locale.LC_ALL, '')


#unicodedata.normalize('NFKD', first.decode('utf8','ignore')).encode('ASCII', 'ignore')

topLevel = "/usr/lib"

def keyget(row):
    return locale.strxfrm(row)
#enddef


def readDir(ldir):
    global topLevel
    mdir = join(topLevel,ldir)
    result = []
    resultnoac = []
    print mdir
    for root, dirs, files in os.walk(mdir):
        print root,dirs, files 
        #sort dirs
        sdirs = sorted(dirs, key=keyget)
        for sdir in sdirs:
            result.append((sdir,'d'))
            noac = unicodedata.normalize('NFKD',sdir.decode('utf8','ignore')).encode('ASCII', 'ignore')
            resultnoac.append(('>' + noac,'d'))
        
        sfiles = sorted(files, key=keyget)
        for sfile in sfiles:
            result.append((sfile,'f'))
            noac = unicodedata.normalize('NFKD',sfile.decode('utf8','ignore')).encode('ASCII', 'ignore')
            resultnoac.append((noac,'f'))
        break
    return result,resultnoac

def printLCD(lcd,lrow,text,shift = 0,padding=40):
    #prepare string
    overlap = len(text) - padding
    if overlap < 0: 
        overlap = 0
        shift = 0
    else:
        shift = shift % overlap
 
    ptext = text[shift:padding].ljust(padding)
    lcd.set_cursor(row=lrow)
    lcd.message(ptext)    
    
def checkVal(val,mx):
    if val < 0:
        val = mx - 1
        rotary.setValue(val*2)
    elif val >= mx:
        val = 0
        rotary.setValue(0)
    
    return val



def main():
    lcd = LCD()
    lcd.set_backlight(True)
    global position
    flist = []
    flistnoac = []
    playing = False
    choosing = False
 
    directory = ""


    rotary.RotaryEncoder(6,10,11)
    old = 0    
    
    #main loop 
    while True:
        butt = rotary.getButton(True)
        val = int(rotary.getValue()/2.)
        if butt and not choosing:
            choosing = True
            #read dir and print first 
            flist,flistnoac = readDir(directory)
            print flistnoac
            rotary.setValue(0)
            val = 0
            old = 0
            printLCD(lcd,1,flistnoac[val][0])
 
        #knob was rotated we want seek or choose file
        if old != val:
            old = val
            if choosing:
                val = checkVal(val,len(flistnoac)) 
                printLCD(lcd,1,flistnoac[val][0])
            

        time.sleep(0.1)   
 
if __name__ == "__main__":
    main()     
