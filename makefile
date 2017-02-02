complie_var = -IC:\Python27\include -g -Wall -c
objects = main.o c_interface.o c_container.o c_list.o c_map.o \
	path_astar.o c_hash.o c_heap.o \

all:	show_path clean
show_path:	$(objects)
	gcc $(objects) \
	-shared -D MS_WIN64 \
	-IC:\Python27\include -IC:\Python27\PC \
	-Lc:\Python27\libs -lpython27 \
	-Wall -g -o show_path

main.o:main.c
	gcc main.c $(complie_var)
c_interface.o:c_interface.c
	gcc c_interface.c $(complie_var)
c_container.o:c_container.c
	gcc c_container.c $(complie_var)
c_list.o:c_list.c
	gcc c_list.c $(complie_var)
c_hash.o:c_hash.c
	gcc c_hash.c $(complie_var)
c_heap.o:c_heap.c
	gcc c_heap.c $(complie_var)
c_map.o:c_map.c
	gcc c_map.c $(complie_var)

path_astar.o:algorithm/path_astar.c
	gcc algorithm/path_astar.c $(complie_var)

 .PHONY : clean
clean:
	-rm all $(objects)
