
vpath % ../../configparser
vpath % ../../log

CFLAGS = -I../../

# cc = gcc
obj = test_configparser.o configparser.o log.o
target = test_configparser

$(target): $(obj)
	cc -o $@ $(obj)

$(obj): %.o: %.c
	cc -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	-rm $(target) $(obj)
