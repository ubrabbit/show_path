/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-28 18:01:04
 *
 */

#include "c_interface.h"

#define PATH_MODE_TEST 0
#define PATH_MODE_ASTAR 1

#define CUR_PATH_MODE PATH_MODE_ASTAR

static Path_Map *g_Map;
static Path_Container *g_Rlt_Container;


static MAP_POS_T Test_Algorithm(MAP_POS_T **result){
        MAP_POS_T total;
        MAP_POS_T row_size,col_size,row,col;
        MAP_POS_T k,idx;

        row_size=g_Map->row_size;
        col_size=g_Map->col_size;

        /*
        MAP_POS_T enter_x, enter_y, exit_x, exit_y;
                 MAP_POS_T *path_map;
        row_size=g_Map->row_size;
        col_size=g_Map->col_size;
        enter_x=g_Map->enter_x;
        enter_y=g_Map->enter_y;
        exit_x=g_Map->exit_x;
        exit_y=g_Map->exit_y;
        */

        total=k=idx=0;
        for(row=0;row<row_size;row++)
                for(col=0;col<col_size;col++){
                        fprintf(stdout, "algorithm: row %ld col %ld color %ld\n",row,col, GetMap_Value(g_Map,row,col) );
                        if(col%2==0 && GetMap_Value(g_Map,row,col)==0 ){
                                (*result)[idx++]=row;
                                (*result)[idx++]=col;
                                total++;
                        }
                        k++;
                }
        fprintf(stdout, "Test_Algorithm total : %ld\n", total);

        return total;
}


static MAP_POS_T Start_Algorithm(MAP_POS_T **result){
        MAP_POS_T total;

        total=0;
        //在此加入算法
        switch(CUR_PATH_MODE){
                case PATH_MODE_ASTAR:
                        total=Start_AStar(g_Map,result);
                        break;
                case PATH_MODE_TEST:
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

        MAP_POS_T row,col;
        MAP_POS_T enter_x, enter_y, exit_x, exit_y;
        Py_ssize_t len,i;
        PyObject *oEnter,*oExit,*posList,*blockList;
        MAP_POS_T pos_row,pos_col,color;
        PyObject* temp;

        assert(NULL==g_Map);

        PyArg_ParseTuple(args,"llOOOO",&row,&col,&oEnter,&oExit,&posList,&blockList);

        enter_x=PyInt_AsLong(PyTuple_GetItem(oEnter,0));
        enter_y=PyInt_AsLong(PyTuple_GetItem(oEnter,1));
        exit_x=PyInt_AsLong(PyTuple_GetItem(oExit,0));
        exit_y=PyInt_AsLong(PyTuple_GetItem(oExit,1));
        if( enter_x<0 || enter_y<0 || exit_x<0 || exit_y<0 ){
                fprintf(stderr, "enter or exit pos error: <0 ! \n");
                Py_RETURN_NONE;
        }
        if( enter_x>=MAP_MAX_ROW || enter_y>=MAP_MAX_COL || exit_x>=MAP_MAX_ROW || exit_y>=MAP_MAX_COL ){
                fprintf(stderr, "enter or exit pos error: >max \n");
                Py_RETURN_NONE;
        }

        g_Map=CreateMap(row,col,enter_x,enter_y,exit_x,exit_y);

        len=PyList_Size(posList);
        for(i=0;i<len;i++){
                temp=PyList_GetItem(posList,i);

                pos_row=PyInt_AsLong(PyTuple_GetItem(temp,0));
                pos_col=PyInt_AsLong(PyTuple_GetItem(temp,1));
                color=PyInt_AsLong(PyTuple_GetItem(temp,2));

                SetMap_Value(g_Map,pos_row,pos_col,color);
                //fprintf(stdout, "SetMap_Value %d %d %ld %ld\n",pos_row,pos_col,color,GetMap_Value(g_Map,pos_row,pos_col));
        }

        len=PyList_Size(blockList);
        for(i=0;i<len;i++){
                temp=PyList_GetItem(blockList,i);

                pos_row=PyInt_AsLong(PyTuple_GetItem(temp,0));
                pos_col=PyInt_AsLong(PyTuple_GetItem(temp,1));

                SetMap_Block(g_Map,pos_row,pos_col);
                //fprintf(stdout, "SetMap_Value %d %d %ld %ld\n",pos_row,pos_col,color,GetMap_Value(g_Map,pos_row,pos_col));
        }

        return Py_BuildValue("i",1);
}


static PyObject* Path_Start(PyObject *self,PyObject *args){
        MAP_POS_T i,row,col,total_unit,size;
        MAP_POS_T *result;
        clock_t start, finish;
        long cost;
        PyObject* tuple_return,*tuple_param;

        if(g_Rlt_Container){
                Free_Path_Container(g_Rlt_Container);
        }
        g_Rlt_Container=NULL;
        if(NULL==g_Map){
                fprintf(stderr, "start fail ,map is not regist! \n");
                Py_RETURN_NONE;
        }

        //size大小为总格子数*2 + 容差偏移
        size=sizeof(MAP_POS_T)*g_Map->row_size*g_Map->col_size*2 + 4*sizeof( MAP_POS_T );
        result=(MAP_POS_T *)malloc( size );
        memset(result,-1,size);

        start=clock()*1000000 / CLOCKS_PER_SEC;;
        total_unit=0;
        total_unit=Start_Algorithm(&result);
        finish=clock()*1000000 / CLOCKS_PER_SEC;
        cost=(long)(finish-start)/1000;

        tuple_return = PyTuple_New(2);
        tuple_param = PyTuple_New(total_unit*2);

        g_Rlt_Container=Create_Container();
        i=0;
        PyTuple_SetItem(tuple_return,0,Py_BuildValue("i",cost));
        while( result[i] != -1 ){
                row=result[i];
                PyTuple_SetItem(tuple_param,i++,Py_BuildValue("i",row));
                col=result[i];
                PyTuple_SetItem(tuple_param,i++,Py_BuildValue("i",col));

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
