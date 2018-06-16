
# [2.1]
# VPATH = ../../configparser:../../log

# [2.2]
# vpath % ../../configparser:../../log

# [2.3]
vpath % ../../configparser
vpath % ../../log


obj = test_configparser.o configparser.o log.o

test_configparser: $(obj)
	gcc -o test_configparser $(obj) -I../../

test_configparser.o: test_configparser.c configparser.h log.h
	gcc -c $< -I../../

configparser.o: configparser.c configparser.h
	gcc -c $< -I../../

log.o: log.c log.h
	gcc -c $< -I../../

.PHONY:clean
clean:
	-rm test_configparser $(obj)
