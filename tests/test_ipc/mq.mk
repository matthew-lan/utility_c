
vpath % ../../log
vpath % ../../ipc

CFLAGS = -I../../ -lpthread -lrt

target = test_mq

obj = test_mq.o log.o
obj += mq_sysv.o mq_sysv_usr.o
obj += mq_posix.o mq_posix_usr.o

$(target): $(obj)
	cc -o $@ $(obj) $(CFLAGS)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
