
vpath % ../../timer
vpath % ../../log

CFLAGS = -I../../ -lpthread -lrt -w

target = test_timer

obj = test_timer.o log.o
obj += timer_fd.o
obj += timer_posix.o

$(target): $(obj)
	cc -o $@ $(obj) $(CFLAGS)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
