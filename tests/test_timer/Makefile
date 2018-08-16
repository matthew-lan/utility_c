
# libutils.so
CFLAGS = -I../../ -Wl,-rpath,../../ -L../../build -lutils -lpthread -lrt

# libutils.a
# CFLAGS = -I../../ -L../../build -lutils -lpthread -lrt

target = test_timer

obj = test_timer.o

$(target): $(obj)
	cc -o $@ $(obj) $(CFLAGS)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
