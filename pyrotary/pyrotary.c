#include <stdio.h>
#include <wiringPi.h>
#include <Python.h>
#include <pthread.h>
 
//#define Button 6        // Pin 22 Orange Pi Zero
//#define EncoderA 10 // Pin 24 Orange Pi Zero
//#define EncoderB 11 // Pin 26 Orange Pi Zero
 
volatile int value = 0;
volatile int button = 0;
pthread_mutex_t lock;

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
    if(my_callback) 
        PyCallback(0);
    
    pthread_mutex_lock(&lock);
    button = 1;
    pthread_mutex_unlock(&lock);

    delay(500);
}
 
void EncoderAInterrupt (void) 
{ 
 right=digitalRead(EncoderB);  //  A=0 und B=1 Erste Impuls bei Rechtslauf
 if(!right && left)       //  A=0 und B=0 Zweite Impuls bei Linkslauf
    {
     if(my_callback)
        PyCallback(-1);
    
     pthread_mutex_lock(&lock);
     value --;
     pthread_mutex_unlock(&lock);
      
    }
 left=0;                       // Zustand vom Ersten Impuls loeschen 
}
 
void EncoderBInterrupt (void) 
{ 
    left= digitalRead(EncoderA);   //  B=0 und A=1 Erste Impuls bei Linkslauf
    if(!left && right)        //  B=0 und A=0 Zweite Impuls bei Rechtslauf
        {
        if(my_callback)
           PyCallback(1);
        pthread_mutex_lock(&lock);
        value ++;
        pthread_mutex_unlock(&lock);
        }
    right=0;                  // Zustand vom Ersten Impuls loeschen
}

static PyObject* getValue(PyObject *self, PyObject *args)
{
    pthread_mutex_lock(&lock);
    PyObject *res = PyLong_FromLong(value);
    pthread_mutex_unlock(&lock);
    return res;
}

static PyObject* setValue(PyObject *self, PyObject *args)
{
    int tmp;
    if (PyArg_ParseTuple(args, "i" , &tmp)){
            pthread_mutex_lock(&lock);
            value = tmp;  
            pthread_mutex_unlock(&lock);  
    }else{
        return NULL;
    }
    Py_INCREF(Py_None);
  return Py_None;
}
 
static PyObject* getButton(PyObject *self, PyObject *args)
{
    pthread_mutex_lock(&lock);
    PyObject *ret = PyLong_FromLong(button);
    pthread_mutex_unlock(&lock);
    return ret;
}

static PyObject* clearButton(PyObject *self, PyObject *args)
{
    pthread_mutex_lock(&lock);
    button = 0;
    pthread_mutex_unlock(&lock);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *rotary(PyObject *self, PyObject *args)
{
 //parse args  
  int Button;
  PyObject *temp = NULL;

  if (PyArg_ParseTuple(args, "iii|O:callback", &Button,&EncoderA,&EncoderB,&temp)) {
      if (temp){
          if (!PyCallable_Check(temp)) {
              PyErr_SetString(PyExc_TypeError, "parameter must be callable");
              return NULL;
          }
          Py_XINCREF(temp);         /* Add a reference to new callback */
          Py_XDECREF(my_callback);  /* Dispose of previous callback */
          my_callback = temp;       /* Remember new callback */
      }      
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
    {"getValue",  getValue, METH_VARARGS,
     "Get value of rotary encoder"},
    {"setValue",  setValue, METH_VARARGS,
     "Set value of rotary encoder"},
    {"getButton",  getButton, METH_VARARGS,
     "Get value of rotary encoder"},
    {"clearButton",  clearButton, METH_VARARGS,
     "Set value of rotary encoder"},
    
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initrotary(void)
{
    (void) Py_InitModule("rotary", RotaryMethods);
    PyEval_InitThreads();
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }
}
