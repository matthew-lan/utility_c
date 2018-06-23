
vpath % ../../timer
vpath % ../../log

CFLAGS = -I../../ -lpthread

target = test_timer

obj = test_timer.o log.o
obj += timer_fd.o

$(target): $(obj)
	cc -o $@ $(obj) $(CFLAGS)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
