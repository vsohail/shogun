#include "lib/config.h"

#ifdef HAVE_PYTHON
#ifndef __PYTHON_H__
#define __PYTHON_H__

#undef _POSIX_C_SOURCE
extern "C" {
#include <Python.h>
#include <numpy/arrayobject.h>
}

#endif //__PYTHON_H__
#endif //HAVE_PYTHON
