all:	show_path clean
show_path:	main.o c_container.o c_list.o c_map.o path_astar.o

main.o:main.c
	gcc main.c -g -Wall -c
c_container.o:path_code/c_container.c
	gcc path_code/c_container.c -g -Wall -c
c_list.o:path_code/c_list.c
	gcc path_code/c_list.c -g -Wall -c
c_map.o:path_code/c_map.c
	gcc path_code/c_map.c -g -Wall -c
path_astar.o:path_code/path_astar.c
	gcc path_code/path_astar.c -g -Wall -c

clean:
	rm *.o
