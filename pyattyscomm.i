%include "docstrings.i"
%module(directors="1") pyattyscomm
%feature("director") AttysCommListener;
%feature("director") AttysCommMessage;
%feature("director") AttysScanListener;
%feature("autodoc","3");
%module pyattyscomm
%{
	#define SWIG_FILE_WITH_INIT
	#include <stdio.h>
	#include "AttysCommBase.h"
	#include "attyscomm/base64.h"
	#include "AttysComm.h"
	#include "AttysScan.h"
%}

%typemap(out) sample_p {
  int i;
  if (NULL == $1) {
	  PyErr_SetString(PyExc_ValueError,"Sample array is NULL. There's no data available.");
	  return NULL;
  }
  $result = PyList_New(10);
  for (i = 0; i < 10; i++) {
    PyObject *o = PyFloat_FromDouble((double) $1[i]);
    PyList_SetItem($result,i,o);
  }
}

/**
%typemap(in,numinputs=0) sample_p (sample_p temp) {
	$1 = temp;
}


%typemap(argout) sample_p s {
  PyObject* list = PyList_New(10);
  for (size_t i = 0; i < 10; ++i) {
	  PyList_SetItem(list, i, PyFloat_FromDouble((float)(temp$argnum[i])));
  }
  $result = list;
}
**/

%include "AttysCommBase.h"
%include "attyscomm/base64.h"
%include "AttysComm.h"
%include "AttysScan.h"
