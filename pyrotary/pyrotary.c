#include <stdio.h>
#include <wiringPi.h>
#include <Python.h>
 
//#define Button 6        // Pin 22 Orange Pi Zero
//#define EncoderA 10 // Pin 24 Orange Pi Zero
//#define EncoderB 11 // Pin 26 Orange Pi Zero
 

int right=0;
int left=0;
int EncoderA,EncoderB;

static PyObject *my_callback = NULL;

void PyCallback(int c){
    PyGILState_STATE state = PyGILState_Ensure();

    PyObject *arglist;
    PyObject *result;
    arglist = Py_BuildValue("(i)", c);
    result = PyObject_CallObject(my_callback, arglist);
    Py_DECREF(arglist);
    Py_DECREF(result);
    PyGILState_Release(state);
}
 
void ButtonInterrupt (void) 
{ 
    PyCallback(0);
    delay (500) ;
}
 
void EncoderAInterrupt (void) 
{ 
 right=digitalRead(EncoderB);  //  A=0 und B=1 Erste Impuls bei Rechtslauf
 if(!right && left)       //  A=0 und B=0 Zweite Impuls bei Linkslauf
    {
     PyCallback(-1);
    }
 left=0;                       // Zustand vom Ersten Impuls loeschen 
}
 
void EncoderBInterrupt (void) 
{ 
left= digitalRead(EncoderA);   //  B=0 und A=1 Erste Impuls bei Linkslauf
if(!left && right)        //  B=0 und A=0 Zweite Impuls bei Rechtslauf
    {
     PyCallback(1);
    }
 right=0;                  // Zustand vom Ersten Impuls loeschen
}

 
static PyObject *rotary (PyObject *self, PyObject *args)
{
 //parse args  
  int Button;
  PyObject *temp;

  if (PyArg_ParseTuple(args, "iiiO:callback", &Button,&EncoderA,&EncoderB,&temp)) {
    if (!PyCallable_Check(temp)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }
    Py_XINCREF(temp);         /* Add a reference to new callback */
    Py_XDECREF(my_callback);  /* Dispose of previous callback */
    my_callback = temp;       /* Remember new callback */
  }else{
    return NULL;
  }

  wiringPiSetup () ;
  pinMode (Button, INPUT);          // PIN als Ausgang konfigurieren
 // pullUpDnControl (Taster, PUD_UP);     // PIN  mit Pul-Upp versehen
  pinMode (EncoderA, INPUT);            // PIN als Ausgang konfigurieren
 // pullUpDnControl (EncoderA, PUD_UP);   // PIN  mit Pul-Upp versehen
  pinMode (EncoderB, INPUT);            // PIN als Ausgang konfigurieren
 // pullUpDnControl (EncoderB, PUD_UP);   // PIN  mit Pul-Upp versehen
 
  wiringPiISR (Button, INT_EDGE_FALLING, &ButtonInterrupt) ;    // Interrupt Routine definieren
  wiringPiISR (EncoderA, INT_EDGE_FALLING, &EncoderAInterrupt); // Interrupt Routine definieren
  wiringPiISR (EncoderB, INT_EDGE_FALLING, &EncoderBInterrupt); // Interrupt Routine definieren
  
  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef RotaryMethods[] = {
    {"RotaryEncoder",  rotary, METH_VARARGS,
     "Init Rotary enc"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initrotary(void)
{
    (void) Py_InitModule("rotary", RotaryMethods);
    PyEval_InitThreads();
}
