/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-22 12:40:18
 *
 */

#include "path_astar.h"

static  AStar_Map *g_Map=NULL;

/*F=G+H
F  :  最终分数
G  :  由出发点到达当前格子的分数
H  :  当前格子到达目的地的预估分数
*/
static int     g_Score_G[MAP_MAX_ROW][MAP_MAX_COL];
static int     g_Score_H[MAP_MAX_ROW][MAP_MAX_COL];

static int     g_MapUnit_Set_Idx;
static int     g_MapUnit_Set[MAP_MAX_ROW][MAP_MAX_COL];

static int     g_Close_List[MAP_MAX_ROW][MAP_MAX_COL];
static long g_TotalSearchGrid=0;


static int Cmp_Pos_Unit(Heap_Unit *node1,Heap_Unit *node2){
        Map_Pos *unit_1,*unit_2;
        MAP_POS_T x1,y1,x2,y2;
        MAP_POS_T unit_score_1,unit_score_2;

        unit_1=(Map_Pos *)node1->data_ptr;
        unit_2=(Map_Pos *)node2->data_ptr;

        x1=unit_1->row;
        y1=unit_1->col;
        x2=unit_2->row;
        y2=unit_2->col;

        unit_score_1 = unit_1->score_f + unit_1->score_temp_f;
        unit_score_2 = unit_2->score_f + unit_2->score_temp_f;

        if( unit_score_1 > unit_score_2 ) return CMP_BIGGER;
        if( unit_score_1 < unit_score_2 ) return CMP_SMALLER;

        if( g_MapUnit_Set[x1][y1] <=g_MapUnit_Set[x2][y2] ) return CMP_SMALLER;

        return CMP_BIGGER;
}


static void MakeMap(Path_Map *map){
        MAP_POS_T i,j,size;
        Map_Pos *unit;

        size=map->row_size * map->col_size ;
        g_Map=(AStar_Map*)malloc( sizeof(AStar_Map) * size );
        g_Map->size=size;
        g_Map->row_size=map->row_size;
        g_Map->col_size=map->col_size;
        g_Map->enter_x=map->enter_x;
        g_Map->enter_y=map->enter_y;
        g_Map->exit_x=map->exit_x;
        g_Map->exit_y=map->exit_y;

        g_Map->map=map;

        g_Map->poslist=(Map_Pos **)malloc( sizeof(Map_Pos*) * size );
        for( i=0; i<map->row_size; i++ ){
                for( j=0; j<map->col_size; j++ ){
                        unit=(Map_Pos*)malloc(sizeof(Map_Pos));
                        unit->row=i;
                        unit->col=j;
                        unit->score_f=0;
                        unit->score_temp_f=0;
                        unit->weight=GetMap_Value(map,i,j);
                        unit->parent=NULL;
                        g_Map->poslist[ i*map->col_size+j ]=unit;
                }
        }

        g_Map->openlist=CreateHeap( size, Cmp_Pos_Unit );
}


static Map_Pos* Map_Index_Unit(MAP_POS_T row ,MAP_POS_T col){
        Map_Pos *unit;

        unit=g_Map->poslist[ row * g_Map->col_size + col ];
        assert( NULL!=unit );
        return unit;
}


static void ReleaseMap(void){
        Map_Pos *unit;
        MAP_POS_T i,j;

        MAP_POS_T idx;

         Heap_Free(g_Map->openlist);
        for( i=0; i<g_Map->row_size; i++ ){
                for( j=0; j<g_Map->col_size; j++ ){
                        idx=i*g_Map->col_size+j;
                        unit=g_Map->poslist[ idx];
                        free(unit);
                        unit->parent=NULL;
                        g_Map->poslist[ idx ]=NULL;
                }
        }

        g_Map->map=NULL;
        free(g_Map->poslist);
        free(g_Map);
        g_Map=NULL;

        memset(g_Score_G,0,sizeof(int) * MAP_MAX_SIZE );
        memset(g_Score_H,0,sizeof(int) * MAP_MAX_SIZE );
        memset(g_Close_List,0,sizeof(int) * MAP_MAX_SIZE );
        g_MapUnit_Set_Idx=0;
        memset(g_MapUnit_Set,0,sizeof(int) * MAP_MAX_SIZE );

        g_TotalSearchGrid=0;

}


static int IsRevertDirect(MAP_POS_T row, MAP_POS_T col, MAP_POS_T next_row, MAP_POS_T next_col){
    MAP_POS_T exit_x,exit_y;

    exit_x=g_Map->exit_x;
    exit_y=g_Map->exit_y;

    if( abs(row-exit_x)+abs(col-exit_y) < abs(next_row-exit_x) + abs(next_col-exit_y) )
        return 1;

    return 0;
}


static int IsPosNextTo(MAP_POS_T row, MAP_POS_T col, MAP_POS_T next_row, MAP_POS_T next_col){
        if( abs(row-next_row)+abs(col-next_col) <=1 )   return 1;
        if ( abs(row-next_row)==1 && abs(col-next_col)==1 )    return 1;
        return 0;
}


static void SearchAroundUnits(MAP_POS_T row, MAP_POS_T col){
        MAP_POS_T i,j,cur_row,cur_col;
        MAP_POS_T exit_x,exit_y;

        Map_Pos *unit;
        int cur_score=g_Score_G[row][col];
        int value_g,value_h;
        int block[3][3];

        Map_Pos *unit_parent;

        exit_x=g_Map->exit_x;
        exit_y=g_Map->exit_y;
        unit_parent=Map_Index_Unit( row,col );

        memset(block,0,sizeof(int)*3*3);
        for(i=-1;i<2;i=i+2){
                cur_row=row+i;
                cur_col=col;
                if( IsMapBlock(g_Map->map, cur_row, cur_col)){
                        block[i+1][1]=1;
                        if ( Get_Map_Pos_Type(g_Map->map,cur_row,cur_col) == BLOCK_T_BLOCK ){
                                block[i+1][0]=1;
                                block[i+1][2]=1;
                        }
                }
        }
        for(j=-1;j<2;j=j+2){
                cur_row=row;
                cur_col=col+j;
                if( IsMapBlock(g_Map->map, cur_row, cur_col)){
                        block[1][j+1]=1;
                        if ( Get_Map_Pos_Type(g_Map->map,cur_row,cur_col) == BLOCK_T_BLOCK ){
                                block[0][j+1]=1;
                                block[2][j+1]=1;
                        }
                }
        }

        for( i=-1; i<2; i++)
                for( j=-1; j<2; j++){

                        if( i==0 && j==0 )
                                continue;

                        g_TotalSearchGrid++;

                        //对应的block数组中的位置
                        if( block[i+1][j+1]>=1 )
                                continue;

                        cur_row=row+i;
                        cur_col=col+j;
                        if( IsMapBlock(g_Map->map, cur_row, cur_col))
                                continue;
                        if( g_Close_List[cur_row][cur_col] >=1 )
                                continue;

                        unit=Map_Index_Unit( cur_row,cur_col );
                        unit->parent=unit_parent;
                        //直角线
                        if( cur_row == row || cur_col == col ){
                                value_g=PATH_COST_1 + unit->weight;
                        }
                        //斜线
                        else{
                                value_g=PATH_COST_2 + unit->weight;
                        }
                        value_g+=cur_score;
                        value_h=( abs( cur_row - exit_x ) + abs( cur_col - exit_y ) ) * PATH_COST_1 * 2;

                        unit->score_f = value_g+value_h;

                        unit->score_temp_f = unit_parent->score_temp_f;
                        //若路径往相反方向，权重应该变大
                        /*
                        if ( IsRevertDirect( unit_parent->row,unit_parent->col, cur_row,cur_col ) ){
                                        unit->score_temp_f += PATH_COST_1;
                                }
                        */

                        g_Score_G[cur_row][cur_col]=value_g;
                        g_Score_H[cur_row][cur_col]=value_h;
                        g_Close_List[cur_row][cur_col]=1;

                        g_MapUnit_Set[cur_row][cur_col]=g_MapUnit_Set_Idx++;
                        //fprintf(stdout, "Heap_Insert %ld  %ld \n",cur_row,cur_col);

                        Heap_Insert(g_Map->openlist,(void*)unit);
                }
}


MAP_POS_T Start_AStar(Path_Map *map, MAP_POS_T **result){
        Map_Pos *unit,*enter_unit,*exit_unit;
        MAP_POS_T enter_x,enter_y,exit_x,exit_y;
        MAP_POS_T cur_row,cur_col;
        MAP_POS_T total;
        int idx;

        if ( NULL== g_Map ){
                MakeMap(map);
        }

        memset(g_Score_G,0,sizeof(int) * MAP_MAX_SIZE );
        memset(g_Score_H,0,sizeof(int) * MAP_MAX_SIZE );
        memset(g_Close_List,0,sizeof(int) * MAP_MAX_SIZE );
        g_MapUnit_Set_Idx=0;
        memset(g_MapUnit_Set,0,sizeof(int) * MAP_MAX_SIZE );

        g_TotalSearchGrid=0;

        enter_x=map->enter_x;
        enter_y=map->enter_y;
        exit_x=map->exit_x;
        exit_y=map->exit_y;

        enter_unit=Map_Index_Unit(enter_x,enter_y);
        exit_unit=Map_Index_Unit(exit_x,exit_y);
        assert( NULL!=enter_unit && NULL!=exit_unit);

        g_Score_G[enter_x][enter_y]=0;
        g_Close_List[enter_x][enter_y]=2;
        g_MapUnit_Set[enter_x][enter_y]=g_MapUnit_Set_Idx++;
        SearchAroundUnits(enter_x,enter_y);

        unit=(Map_Pos*)Heap_GetTop(g_Map->openlist);
        while( unit ){
                cur_row=unit->row;
                cur_col=unit->col;
                //fprintf(stdout, "Cur Choose %ld  %ld \n",cur_row,cur_col);

                if( cur_row == exit_x && cur_col == exit_y ){
                        //fprintf(stdout, "succ !!!!!!!!!!!!!!! \n");
                        break;
                }

                g_Close_List[cur_row][cur_col]=2;
                Heap_Pop_Free(g_Map->openlist);
                SearchAroundUnits(cur_row, cur_col);

                unit=(Map_Pos*)Heap_GetTop(g_Map->openlist);

        }


        total=0;
        unit=exit_unit;
        idx=0;
        fprintf(stdout, "finsh search final_score = %ld search_grid = %ld \n",g_Score_G[exit_x][exit_y],g_TotalSearchGrid );
        while(unit){
                cur_row=unit->row;
                cur_col=unit->col;

                //fprintf(stdout, "result 0000 is  %ld %ld \n",cur_row,cur_col);
                if( cur_row == exit_x && cur_col == exit_y ){
                        unit=unit->parent;
                        continue;
                }
                if( cur_row == enter_x && cur_col == enter_y ){
                        unit=unit->parent;
                        continue;
                }

                //fprintf(stdout, "result 111111 is  %ld %ld \n",cur_row,cur_col);
                (*result)[idx++]=cur_row;
                 (*result)[idx++]=cur_col;
                total++;
                unit=unit->parent;
        }

        ReleaseMap();
        //fprintf(stdout, "Start_AStar total: %ld\n",total);
        return total;
}
