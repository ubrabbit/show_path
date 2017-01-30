/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-28 18:01:04
 *
 */

#ifdef _WIN32
        #define PYTHON_PATH <Python.h>
#else
        #define PYTHON_PATH "python2.7/Python.h"
#endif

#include PYTHON_PATH
