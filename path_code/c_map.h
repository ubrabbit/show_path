/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-24 12:01:15
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#ifndef DEFINED_MAP
#define DEFINED_MAP


typedef struct Path_Map{
        int row_size;
        int col_size;

        int enter_x;
        int enter_y;
        int exit_x;
        int exit_y;

        int *path_map;
}Path_Map;


Path_Map* CreateMap(int row,int col,int enter_x,int enter_y,int exit_x,int exit_y);

void FreeMap(Path_Map *map);

int GetMap_Value(Path_Map *map,int row,int col);

void SetMap_Value(Path_Map *map,int row,int col,int value);


#endif


