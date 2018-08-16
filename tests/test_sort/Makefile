
# libutils.so
CFLAGS = -I../../ -Wl,-rpath,../../ -L../../build -lutils -lpthread -lrt

target = test_sort

obj = test_sort.o

$(target): $(obj)
	cc -o $@ $(obj) $(CFLAGS)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
