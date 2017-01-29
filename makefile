all:	show_path clean
show_path:	main.o c_interface.o c_container.o c_list.o c_map.o path_astar.o c_hash.o c_heap.o
	gcc main.o c_container.o c_list.o c_map.o path_astar.o \
	c_hash.o c_heap.o c_interface.o \
	-I/usr/include/python2.7/ \
	-lpython2.7 \
	-Wall -g -o show_path

main.o:main.c
	gcc main.c -g -Wall -c
c_interface.o:c_interface.c
	gcc c_interface.c -g -Wall -c
c_container.o:c_container.c
	gcc c_container.c -g -Wall -c
c_list.o:c_list.c
	gcc c_list.c -g -Wall -c
c_hash.o:c_hash.c
	gcc c_hash.c -g -Wall -c
c_heap.o:c_heap.c
	gcc c_heap.c -g -Wall -c
c_map.o:c_map.c
	gcc c_map.c -g -Wall -c

path_astar.o:algorithm/path_astar.c
	gcc algorithm/path_astar.c -g -Wall -c

clean:
	rm *.o
