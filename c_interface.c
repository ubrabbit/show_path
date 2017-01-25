/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-24 20:35:28
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include <Python.h>

#include "path_code/c_map.h"
#include "path_code/c_container.h"

#include "path_code/path_astar.h"


#define PATH_MODE_ASTAR 1

#define CUR_PATH_MODE PATH_MODE_ASTAR


static Path_Map *g_Map;
static Path_Container *g_Rlt_Container;

static int Test_Algorithm(int **result){
        int total;
        int row_size,col_size,row,col;
        int enter_x, enter_y, exit_x, exit_y;
        int *path_map;

        row_size=g_Map->row_size;
        col_size=g_Map->col_size;
        enter_x=g_Map->enter_x;
        enter_y=g_Map->enter_y;
        exit_x=g_Map->exit_x;
        exit_y=g_Map->exit_y;
        path_map=g_Map->path_map;

        int i,j,k,idx;
        k=idx=0;
        for(i=0;i<row_size;i++)
                for(j=0;j<col_size;j++){
                        k++;
                        if(k%2==0){
                                (*result)[idx++]=i;
                                (*result)[idx++]=j;
                                total++;
                        }
                }

        return total;
}


static int Start_Algorithm(int **result){
        int total;

        total=0;
        //在此加入算法
        switch(CUR_PATH_MODE){
                case PATH_MODE_ASTAR:
                        total=Test_Algorithm(result);
                        break;
                default:
                        break;
        }

        return total;
}


static PyObject* Init_Path(PyObject *self,PyObject *args){

        g_Map=NULL;
        g_Rlt_Container=NULL;

        Py_RETURN_NONE;
}


static PyObject* Reset_Path(PyObject *self,PyObject *args){
        if(g_Map){
                FreeMap(g_Map);
        }
        g_Map=NULL;

        if(g_Rlt_Container){
                Free_Path_Container(g_Rlt_Container);
        }
        g_Rlt_Container=NULL;

        Py_RETURN_NONE;
}


static PyObject* Regist_Map(PyObject *self,PyObject *args){
        int row,col;
        long enter_x, enter_y, exit_x, exit_y;
        long len;

        PyObject *oEnter,*oExit,*oList;

        fprintf(stdout, "Regist_Map ------------11111111111111111\n");
        PyArg_ParseTuple(args,"iiOOO",&row,&col,&oEnter,&oExit,&oList);

        fprintf(stdout, "Regist_Map ------------2222222222222222\n" );
        enter_x=PyInt_AsLong(PyTuple_GetItem(oEnter,0));
        enter_y=PyInt_AsLong(PyTuple_GetItem(oEnter,1));
        exit_x=PyInt_AsLong(PyTuple_GetItem(oExit,0));
        exit_y=PyInt_AsLong(PyTuple_GetItem(oExit,1));
        fprintf(stdout, "Regist_Map ------------333333333333333333\n");

        g_Map=CreateMap(row,col,enter_x,enter_y,exit_x,exit_y);
        fprintf(stdout, "Regist_Map ------------44444444444444 %d %d %ld %ld %ld %ld \n",row,col,enter_x,enter_y,exit_x,exit_y);

        int i;
        long pos_row,pos_col,color;
        PyObject* temp;
        len=PyList_Size(oList);
        for(i=0;i<len;i++){
                temp=PyList_GetItem(oList,i);

                pos_row=PyInt_AsLong(PyTuple_GetItem(temp,0));
                pos_col=PyInt_AsLong(PyTuple_GetItem(temp,1));
                color=PyInt_AsLong(PyTuple_GetItem(temp,2));

                SetMap_Value(g_Map,pos_row,pos_col,color);
        }
        fprintf(stdout, "Regist_Map ------------66666666666666666666\n" );

        Py_RETURN_NONE;
}


static PyObject* Path_Start(PyObject *self,PyObject *args){

        int i,row,col,total_unit;
        int *result;
        clock_t start, finish;
        long cost,size;

        assert(NULL != g_Map);
        assert(NULL == g_Rlt_Container);

        //size大小为总格子数*2 + 容差偏移
        size=sizeof(int)*g_Map->row_size*g_Map->col_size*2 + 4*sizeof(int);
        result=(int *)malloc( size );
        memset(result,-1,size);

        start=clock();
        total_unit=0;
        total_unit=Start_Algorithm(&result);
        finish=clock();
        cost=(long)(finish-start)/CLOCKS_PER_SEC;

        PyObject* tuple_return = PyTuple_New(2);
        PyObject* tuple_param = PyTuple_New(total_unit*2);

        g_Rlt_Container=Create_Container();
        i=0;
        PyTuple_SetItem(tuple_return,0,Py_BuildValue("l",cost));
        while( result[i] != -1 ){
                row=result[i];
                PyTuple_SetItem(tuple_param,i,Py_BuildValue("l",row));
                i++;
                col=result[i];
                PyTuple_SetItem(tuple_param,i,Py_BuildValue("l",col));
                i++;

                Container_Add_Unit(g_Rlt_Container,row,col);
        }
        g_Rlt_Container->cost=cost;
        PyTuple_SetItem(tuple_return,1,tuple_param);

        free(result);
        return tuple_return;
}


static PyMethodDef PathMethods[] = {
        {"Init_Path",Init_Path, METH_VARARGS, "Init_Path"},
        {"Reset_Path",Reset_Path, METH_VARARGS, "Reset_Path"},
        {"Regist_Map", Regist_Map, METH_VARARGS, "Regist_Map"},
        {"Path_Start", Path_Start, METH_VARARGS, "Path_Start"},
        {NULL,NULL,0,NULL},
};


PyMODINIT_FUNC initc_path(void){
        (void)Py_InitModule("c_path",PathMethods);
}
