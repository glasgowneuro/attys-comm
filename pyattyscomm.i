%include "docstrings.i"
%module(directors="1") pyattyscomm
%feature("director") SampleCallback;
%feature("autodoc","3");
%module pyattyscomm
%{
	#define SWIG_FILE_WITH_INIT
	#include <stdio.h>
	#include "AttysCommBase.h"
	#include "attyscomm/base64.h"
	#include "AttysComm.h"
	#include "AttysScan.h"
	#include <array>
    #define N_CHANS 10
%}

%include "std_array.i"

namespace std {
  %template(FloatArray) array<float,N_CHANS>;
}

%inline %{
	class SampleCallback{
	public:
	virtual void hasSample(double t,const std::array<float,N_CHANS> &data) = 0;
	virtual ~SampleCallback() {};
	SampleCallback() {};
};
%}

%typemap(out) sample_p {
  int i;
  if (NULL == $1) {
	  PyErr_SetString(PyExc_ValueError,"Sample array is NULL. There's no data available.");
	  return NULL;
  }
  $result = PyList_New(N_CHANS);
  for (i = 0; i < N_CHANS; i++) {
    PyObject *o = PyFloat_FromDouble((double) $1[i]);
    PyList_SetItem($result,i,o);
  }
}

%include "AttysCommBase.h"
%include "attyscomm/base64.h"
%include "AttysComm.h"
%include "AttysScan.h"


%inline %{
struct MyAttysCommListener : AttysCommListener {
	SampleCallback* callback = NULL;
	virtual void hasSample(double t, sample_p sample) {
		if (callback) {
			std::array<float,N_CHANS> data;
			for(int i = 0; i < N_CHANS; i++) {
				data[i] = sample[i];
			}
			callback->hasSample(t,data);
		}
	}
};

MyAttysCommListener myAttysCommListener;

void connectCallback(AttysCommBase& attysCommBase, SampleCallback* cb) {
	myAttysCommListener.callback = cb;
	attysCommBase.registerCallback(&myAttysCommListener);
}

void disConnectCallback(AttysCommBase& attysCommBase) {
	myAttysCommListener.callback = NULL;
	attysCommBase.unregisterCallback();
}	
%}
