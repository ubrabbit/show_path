/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-22 12:40:18
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../c_map.h"
#include "../c_list.h"
#include "../c_heap.h"


#ifndef DEFINED_ASTAR
#define DEFINED_ASTAR

#define PATH_COST_1 10
#define PATH_COST_2 14

typedef struct Map_Pos Map_Pos;
struct Map_Pos{

        MAP_POS_T       row;
        MAP_POS_T       col;
        MAP_POS_T       score_f;
        MAP_POS_T       score_temp_f;
        MAP_POS_T       weight;
        Map_Pos        *parent;
};

typedef struct AStar_Map{
        MAP_POS_T size;
        MAP_POS_T row_size;
        MAP_POS_T col_size;

        MAP_POS_T enter_x;
        MAP_POS_T enter_y;
        MAP_POS_T exit_x;
        MAP_POS_T exit_y;

        Map_Pos **poslist;
        Heap_Base        *openlist;

        Path_Map        *map;
}AStar_Map;


MAP_POS_T Start_AStar(Path_Map *map, MAP_POS_T **result);

#endif

