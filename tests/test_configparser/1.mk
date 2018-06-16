
obj = test_configparser.o configparser.o log.o

test_configparser: $(obj)
	gcc -o test_configparser $(obj) -I../../

test_configparser.o: test_configparser.c ../../configparser/configparser.h ../../log/log.h
	gcc -c test_configparser.c -I../../

configparser.o: ../../configparser/configparser.c ../../configparser/configparser.h
	gcc -c ../../configparser/configparser.c -I../../

log.o: ../../log/log.c ../../log/log.h
	gcc -c ../../log/log.c -I../../

.PHONY:clean
clean:
	-rm test_configparser $(obj)
