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
#include "c_map.h"

#ifndef DEFINED_CONTAINER
#define DEFINED_CONTAINER


typedef struct Path_Unit{
        MAP_POS_T row;
        MAP_POS_T col;
}Path_Unit;


typedef struct  Path_Container{
        long cost;
        MAP_POS_T total_size;
        Link_List *path_list;
}Path_Container;


Path_Container* Create_Container(void);

Path_Unit* Create_Path_Unit(MAP_POS_T row,MAP_POS_T col);

void Free_Path_Unit(Path_Unit *unit);

void Free_Path_Container(Path_Container *container);

int Container_Add_Unit(Path_Container *container,MAP_POS_T row, MAP_POS_T col);

Path_Unit* Container_Pop_Unit(Path_Container *container);

#endif
