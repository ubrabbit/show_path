/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-24 12:01:05
 *
 */

#include "c_container.h"


Path_Container* Create_Container(){
        Path_Container *container;

        container=(Path_Container*)malloc(sizeof(Path_Container));
        container->cost=0;
        container->total_size=0;
        container->path_list=CreateList();

        return container;
}


Path_Unit* Create_Path_Unit(int row,int col){
        Path_Unit *unit;

        unit=(Path_Unit*)malloc(sizeof(Path_Unit));
        unit->row=row;
        unit->col=col;

        return unit;
}


void Free_Path_Unit(Path_Unit *unit){
        free(unit);
}


void Free_Path_Container(Path_Container *container){
        Path_Unit *unit;

        unit=ListPopNode(container->path_list);
        while(unit){
                Free_Path_Unit(unit);
                unit=ListPopNode(container->path_list);
        }

        container->total_size=0;
        ListFreeAll(container->path_list);
        free(container);
}


int Container_Add_Unit(Path_Container *container,int row,int col){
        Path_Unit *unit=Create_Path_Unit(row,col);

        container->total_size++;
        ListAddNode(container->path_list,(void *)unit);

        return 1;
}


Path_Unit* Container_Pop_Unit(Path_Container *container){
        Path_Unit *unit=ListPopNode(container->path_list);
        container->total_size--;

        return unit;
}
