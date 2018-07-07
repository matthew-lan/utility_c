
# libutils.so
CFLAGS = -I../../ -Wl,-rpath,../../ -L../../ -lutils -lpthread -lrt

target = test_mq

obj = test_mq.o

$(target): $(obj)
	cc -o $@ $(obj) $(CFLAGS)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
