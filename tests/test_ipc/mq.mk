
vpath % ../../log
vpath % ../../ipc

CFLAGS = -I../../ -lpthread

target = test_mq

obj = test_mq.o log.o
obj += mq_sysv.o

$(target): $(obj)
	cc -o $@ $(obj) $(CFLAGS)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
