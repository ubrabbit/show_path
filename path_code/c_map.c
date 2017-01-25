/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-24 12:01:22
 *
 */

#include "c_map.h"


Path_Map* CreateMap(int row,int col,int enter_x,int enter_y,int exit_x,int exit_y){
        Path_Map *map;

        map=(Path_Map*)malloc(sizeof(Path_Map));
        map->row_size=row;
        map->col_size=col;

        map->enter_x=enter_x;
        map->enter_y=enter_y;
        map->exit_x=exit_x;
        map->exit_y=exit_y;

        map->path_map=(int*)malloc( sizeof(int) * row * col );

        int i,j;
        for (i=0; i<row; i++)
                for( j=0; j<col; j++){
                        SetMap_Value(map,i,j,0);
                }

        return map;
}


void FreeMap(Path_Map *map){
        free(map->path_map);
        free(map);
}


int GetMap_Value(Path_Map *map,int row,int col){
        assert(row*col < map->row_size*map->col_size );

        return  map->path_map[row*col];
}


void SetMap_Value(Path_Map *map,int row,int col,int value){
        map->path_map[row*col]=value;
}

