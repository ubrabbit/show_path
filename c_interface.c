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
        long enter_x,int enter_y,int exit_x,int exit_y;
        long len;

        PyObject *oEnter,*oExit,*oList;

        PyArg_ParseTuple(args,"iiOOO",&row,&col,&oEnter,&oExit,&oList);

        enter_x=PyTuple_GetItem(oEnter,0);
        enter_y=PyTuple_GetItem(oEnter,1);
        exit_x=PyTuple_GetItem(oExit,0);
        exit_y=PyTuple_GetItem(oExit,1);

        g_Map=CreateMap(int row,int col,int enter_x,int enter_y,int exit_x,int exit_y);

        int i;
        long pos_row,pos_col,color;
        PyObject* temp;
        len_lst=PyList_Size(oList);
        for(i=0;i<len;i++){
                temp=PyList_GetItem(oList,i);

                pos_row=PyTuple_GetItem(temp,0);
                pos_col=PyTuple_GetItem(temp,1);
                color=PyTuple_GetItem(temp,2);

                SetMap_Value(g_Map,pos_row,pos_col,color);
        }

        Py_RETURN_NONE;
}


static PyObject* Path_Start(PyObject *self,PyObject *args){

        int i,row,col,total_unit;
        int *result;
        clock_t start, finish;
        long cost;

        assert(NULL != g_Map);
        assert(NULL == g_Rlt_Container);

        //size大小为总格子数*2 + 容差偏移
        size=sizeof(int)*g_Map->row_size*g_Map->col_size*2 + 4*sizeof(int);
        result=(int *)malloc( size );
        memset(result,-1,size);

        start=clock();
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


static int Start_Algorithm(int **result){

        int i,row,col,total;
        int *result;

        total=0;
        //在此加入算法
        switch(CUR_PATH_MODE){
                case PATH_MODE_ASTAR:
                        break;
                default:
                        break;
        }

        return total;
}


static PyMethodDef PathMethods[] = {
        {"Init_Path"},Init_Path, METH_VARARGS, "Init_Path"},
        {"Reset_Path"},Reset_Path, METH_VARARGS, "Reset_Path"},
        {"Regist_Map", Regist_Map, METH_VARARGS, "Regist_Map"},
        {"Path_Start", Path_Start, METH_VARARGS, "Path_Start"},
        {NULL,NULL,0,NULL},
};


PyMODINIT_FUNC initpath(void){
        (void)Py_InitModule("c_path",PathMethods);
}


int test_path(void){

}
