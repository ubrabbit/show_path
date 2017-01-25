/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-24 12:00:47
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_list.h"


#ifndef DEFINED_CONTAINER
#define DEFINED_CONTAINER


typedef struct Path_Unit{
        int row;
        int col;
}Path_Unit;


typedef struct  Path_Container{
        long cost;
        int total_size;
        Link_List *path_list;
}Path_Container;


Path_Container* Create_Container(void);

Path_Unit* Create_Path_Unit(int row,int col);

void Free_Path_Unit(Path_Unit *unit);

void Free_Path_Container(Path_Container *container);

int Container_Add_Unit(Path_Container *container,int row,int col);

Path_Unit* Container_Pop_Unit(Path_Container *container);

#endif
