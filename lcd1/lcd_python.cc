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
            .def_readonly("lost" , &TextMetrics::lost)
            .def_readonly("count" , &TextMetrics::count);


          class_<TTFont>("TTFont", init<const std::string&, int>());

          class_<Glcd>("Glcd")
              .def("init", &Glcd::init)
              .def("setFont", &Glcd::setFont)
              .def("clear", &Glcd::clear)
              .def("putpixel", &Glcd::putpixel)
              .def("redraw", &Glcd::redraw)
              .def("fillRect", &Glcd::fillRect)
              .def("drawString", &Glcd::drawString)
              .def("drawBitmap", static_cast<void (Glcd::*)(int, int, int, int, const std::string&)>(&Glcd::drawBitmap))
              .def("drawBitmap", static_cast<void (Glcd::*)(int, int, int, int, int, const std::string&)>(&Glcd::drawBitmap))
              .def("setBacklight", &Glcd::setBacklight)
              .def("getWidth", &Glcd::getWidth)
              .def("getHeight", &Glcd::getHeight)
		      .def("line",&Glcd::line)
			  .def("lineH",&Glcd::lineH)
			  .def("lineV",&Glcd::lineV)
			  .def("drawRect",&Glcd::drawRect);

}
