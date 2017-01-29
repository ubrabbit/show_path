/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-24 12:01:22
 *
 */

#include "c_map.h"


Path_Map* CreateMap(MAP_POS_T row, MAP_POS_T col, MAP_POS_T enter_x, MAP_POS_T enter_y, MAP_POS_T exit_x, MAP_POS_T exit_y){
        Path_Map *map;

        assert(row*col <= MAP_MAX_SIZE);

        map=(Path_Map*)malloc(sizeof(Path_Map));
        map->row_size=row;
        map->col_size=col;

        map->enter_x=enter_x;
        map->enter_y=enter_y;
        map->exit_x=exit_x;
        map->exit_y=exit_y;

        map->path_map=(MAP_POS_T*)malloc( sizeof(MAP_POS_T) * row * col );

        MAP_POS_T i,j;
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


MAP_POS_T GetMap_Index(Path_Map *map, MAP_POS_T row,MAP_POS_T col){
        MAP_POS_T col_size=map->col_size;

        return row*col_size + col;
}


MAP_POS_T GetMap_Value(Path_Map *map,MAP_POS_T row,MAP_POS_T col){
        MAP_POS_T row_index=GetMap_Index(map,row,col);

        assert( row_index < map->row_size*map->col_size );
        return  map->path_map[row_index];
}


void SetMap_Block(Path_Map *map,MAP_POS_T row, MAP_POS_T col){
        MAP_POS_T row_index=GetMap_Index(map,row,col);

        assert( row_index < map->row_size * map->col_size );
        map->path_map[ row_index ]=BLOCK_VALUE;
}


void SetMap_Value(Path_Map *map, MAP_POS_T row, MAP_POS_T col, MAP_POS_T value){
        MAP_POS_T row_index=GetMap_Index(map,row,col);

        assert( row_index < map->row_size * map->col_size );
        map->path_map[ row_index ]=value;
}


int Get_Map_Pos_Type(Path_Map *map,MAP_POS_T row, MAP_POS_T col){
        if ( row<0 || col<0 )   return BLOCK_T_BORDER;
        if ( row>=map->row_size || col>=map->col_size ) return BLOCK_T_BORDER;

        MAP_POS_T value;
        value=GetMap_Value(map,row,col);
        if( value==BLOCK_VALUE )        return BLOCK_T_BLOCK;

        return BLOCK_T_NORMAL;
}


int IsMapBlock(Path_Map *map,MAP_POS_T row, MAP_POS_T col){

        if( Get_Map_Pos_Type(map,row,col) != BLOCK_T_NORMAL )       return 1;

        return 0;
}
