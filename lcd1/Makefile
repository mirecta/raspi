# location of the Python header files

PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
PYTHON_BOOST = py27

# location of the Boost Python include files and library

BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

# compile mesh classes
TARGET = lcd

$(TARGET).so: lcd.o lcd_python.o
		g++ -shared -Wl,--export-dynamic lcd.o lcd_python.o -L$(BOOST_LIB) -lboost_python-$(PYTHON_BOOST) -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -o $(TARGET).so  -l bcm2835  `freetype-config --libs` 

%.o: %.cc
		g++  -std=c++11 `freetype-config --cflags` -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $< -o $@

