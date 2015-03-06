#!/usr/bin/python





table = {'0':'0',
         '1':'8',
         '2':'4',
         '3':'c',
         '4':'2',
         '5':'a',
         '6':'6',
         '7':'e',
         '8':'1',
         '9':'9',
         'A':'5',
         'B':'d',
         'C':'3',
         'D':'b',
         'E':'7',
         'F':'f'
         }




def main():
    """
    """
    inchar = 0;
    charid = 0;
    fromchar = 32;
    tochar = 128;
    first = 0
    name = "big"
    font = "10x20.bdf"  
  
    print "//%s" % font
    print "const char font_%s[] = { " % name

    fl = open(font)
    for line in fl.xreadlines():
        if inchar == 0:
            data = line.split(' ')
            if data[0] == 'STARTCHAR':
                inchar = 1
                continue
            #endif
        #endif
        if inchar == 1:
            data = line.split(' ')
            if data[0] == 'ENCODING':
                charnr = int(data[1])
                if charnr < fromchar or charnr > tochar:
                    inchar = 0
                    continue
                else:
                    print "//char no %d" % charnr;
                    inchar = 2
                    continue
                #endif
            #endif
        #eindif
        if inchar == 2:
            if line.strip() == 'BITMAP':
                inchar = 3
                continue
            #endif
        #endif
        if inchar == 3:
            if line.strip() == 'ENDCHAR':
                inchar = 0
                continue
            else:
                if first == 0:
                    first = 1
                    append = ''
                else:
                    append = ','
                #endif
                data = line.strip()
                if len(data) == 4:
                    print "%s0x%c%c, 0x%c%c" % (append,table[data[1]],table[data[0]],table[data[3]],table[data[2]])
                elif len(data) == 2:
                    print "%s0x%c%c" % (append,table[data[1]],table[data[0]])

            #endif
        #endif
    #endfor
    print "};"

#enddef




if __name__ == "__main__":
    main()
#edif

