
BUILDPATH = ./build

PREBUILD = prebuild
LIB_A    = libutils.a
LIB_SO   = libutils.so


lib_SRCPATH  = configparser
lib_SRCPATH += ipc
lib_SRCPATH += list
lib_SRCPATH += log
lib_SRCPATH += sort
lib_SRCPATH += timer


lib_SOURCES = $(foreach dir, ${lib_SRCPATH}, $(wildcard ${dir}/*.c))
lib_OBJS    = $(patsubst %.c, %.o, ${lib_SOURCES})

CFLAGS  = -I./  #-w
LDFLAGS = -lpthread -lrt


.PHONY:all
all: ${PREBUILD} ${LIB_A} ${LIB_SO}

${PREBUILD}:
	-mkdir -p $(foreach dir, ${lib_SRCPATH}, ${BUILDPATH}/${dir})

${LIB_A}: ${lib_OBJS}
	ar r $@ $(foreach obj, $^, $(wildcard ${BUILDPATH}/${obj}))

${LIB_SO}: ${lib_OBJS}
	gcc -g -shared -o $@ $(foreach obj, $^, $(wildcard ${BUILDPATH}/${obj}))

${lib_OBJS}: %.o: %.c
	gcc -g -c -fPIC -o ${BUILDPATH}/$@ $< ${CFLAGS} ${LDFLAGS}


.PHONY:clean
clean:
	-rm -rf ${LIB_A} ${LIB_SO} ${BUILDPATH}
