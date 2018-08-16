
top_srcdir   = .
top_builddir = ${top_srcdir}/build

LIB_A    = libutils.a
LIB_SO   = libutils.so


lib_SRCPATH += ${top_srcdir}/configparser
lib_SRCPATH += ${top_srcdir}/ipc
lib_SRCPATH += ${top_srcdir}/list
lib_SRCPATH += ${top_srcdir}/log
lib_SRCPATH += ${top_srcdir}/sort
lib_SRCPATH += ${top_srcdir}/timer


lib_SOURCES = $(foreach dir, ${lib_SRCPATH}, $(wildcard ${dir}/*.c))
lib_OBJS    = $(patsubst ${top_srcdir}/%.c, ${top_builddir}/%.o, ${lib_SOURCES})

lib_CFLAGS  += -I./
lib_CFLAGS  += -Wno-int-to-pointer-cast
lib_CFLAGS  += -Wno-pointer-to-int-cast

lib_LDADD   += -lpthread
lib_LDADD   += -lrt

CFLAGS = -g

CC     = gcc
AR     = ar

.PHONY:all
all: ${LIB_A} ${LIB_SO}

${LIB_A}: ${lib_OBJS}
	${AR} r ${top_builddir}/$@ $^

${LIB_SO}: ${lib_OBJS}
	${CC} ${CFLAGS} -shared -o ${top_builddir}/$@ $^

${lib_OBJS}: ${top_builddir}/%.o: ${top_srcdir}/%.c
	@-mkdir -p ${dir $@}
	${CC} ${CFLAGS} -c -fPIC -o $@ $< ${lib_CFLAGS} ${lib_LDADD}


.PHONY:clean
clean:
	-rm -rf ${top_builddir}
