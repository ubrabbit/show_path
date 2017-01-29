/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-24 20:35:28
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "c_interface.h"

#define PYTHON_CHECK_NULL(value) if(!value){PyErr_Print();}
#define PYTHON_CHECK_NULL_EXIT(value) if(!value){PyErr_Print();exit(-1);}


int main(void){
        Py_Initialize();

        initc_path();

        PyObject *pModule,*pFunc;
        pModule=PyImport_Import(PyString_FromString("init"));
        PYTHON_CHECK_NULL(pModule);
        pFunc=PyObject_GetAttrString(pModule,"start");
        PYTHON_CHECK_NULL(pFunc);

        PYTHON_CHECK_NULL( PyObject_CallObject(pFunc,NULL) );

        Py_Finalize();

        return 0;
}

