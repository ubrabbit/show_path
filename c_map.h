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

#define MAP_POS_T  long
#define MAP_MAX_ROW 1024
#define MAP_MAX_COL 1024
#define MAP_MAX_SIZE (MAP_MAX_ROW * MAP_MAX_COL)

#define BLOCK_VALUE     9999999

#define BLOCK_T_NORMAL 0
#define BLOCK_T_BORDER 1
#define BLOCK_T_BLOCK 2


typedef struct Map_Unit{
        MAP_POS_T row;
        MAP_POS_T col;

}Map_Unit;


typedef struct Path_Map{
        MAP_POS_T row_size;
        MAP_POS_T col_size;

        MAP_POS_T enter_x;
        MAP_POS_T enter_y;
        MAP_POS_T exit_x;
        MAP_POS_T exit_y;

        MAP_POS_T *path_map;
}Path_Map;


typedef struct Path_Record{
        MAP_POS_T idx;
        MAP_POS_T unit_cnt;
        MAP_POS_T pos_record[MAP_MAX_SIZE*2];
}Path_Record;


Path_Map* CreateMap(MAP_POS_T row,MAP_POS_T col,MAP_POS_T enter_x,MAP_POS_T enter_y,MAP_POS_T exit_x,MAP_POS_T exit_y);

void FreeMap(Path_Map *map);

MAP_POS_T GetMap_Value(Path_Map *map,MAP_POS_T row, MAP_POS_T col);

void SetMap_Value(Path_Map *map,MAP_POS_T row, MAP_POS_T col, MAP_POS_T value);

void SetMap_Block(Path_Map *map,MAP_POS_T row, MAP_POS_T col);

int Get_Map_Pos_Type(Path_Map *map,MAP_POS_T row, MAP_POS_T col);

int IsMapBlock(Path_Map *map,MAP_POS_T row, MAP_POS_T col);


#endif


