
vpath % ../../sort
vpath % ../../log

CFLAGS = -I../../

target = test_sort

obj = test_sort.o log.o
obj += bubble_sort.o
obj += cocktail_sort.o
obj += select_sort.o
obj += insert_sort.o
obj += dichinsert_sort.o
obj += shell_sort.o
obj += merge_sort.o
obj += heap_sort.o
obj += quick_sort.o

$(target): $(obj)
	cc -o $@ $(obj)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)