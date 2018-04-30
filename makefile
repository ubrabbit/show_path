CC = gcc
CFLAGS = -fPIC -g -Wall -c
INCLUDE_PY = `python2.7-config --includes --libs`

objects = c_interface.o c_container.o c_list.o c_map.o c_hash.o c_heap.o path_astar.o

all:	c_path clean

c_path:	$(objects)
	gcc $(objects) \
	${INCLUDE_PY} \
	-shared \
	-o c_path.so

c_interface.o:c_interface.c
	gcc c_interface.c $(CFLAGS)
c_container.o:c_container.c
	gcc c_container.c $(CFLAGS)
c_list.o:c_list.c
	gcc c_list.c $(CFLAGS)
c_hash.o:c_hash.c
	gcc c_hash.c $(CFLAGS)
c_heap.o:c_heap.c
	gcc c_heap.c $(CFLAGS)
c_map.o:c_map.c
	gcc c_map.c $(CFLAGS)

path_astar.o:algorithm/path_astar.c
	gcc algorithm/path_astar.c $(CFLAGS)

.PHONY : clean
clean:
	-rm $(objects)
