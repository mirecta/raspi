#include "lcd.h"

    
// g++ -std=c++11 lcd.cc -o lcd `freetype-config --cflags` -l bcm2835  `freetype-config --libs`


/*

   g++ -std=c++11 -c lcd.cc -o lcd.o `freetype-config --cflags`
   g++ -std=c++11 -c test.cc -o test.o `freetype-config --cflags`
   g++ -std=c++11 -o lcd  lcd.o test.o -l bcm2835  `freetype-config --libs`
   */

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(lcd)
{
          using namespace boost::python;
          class_<TextMetrics>("TextMetrics")
            .def_readonly("width", &TextMetrics::width)
            .def_readonly("height", &TextMetrics::height)
            .def_readonly("baseline", &TextMetrics::baseline)
            .def_readonly("lost" , &TextMetrics::lost);



}
