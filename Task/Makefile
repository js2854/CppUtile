TARGET  = bin/demo_task
INC_DIR = . #inc
SRC_DIR = . #src
OBJ_DIR = obj

CFLAGS  = -Wall -g
LDFLAGS = -lpthread -lrt

SRCS = $(foreach DIR, ${SRC_DIR}, $(wildcard ${DIR}/*.cpp))
INCS = $(addprefix -I, ${INC_DIR})
OBJS = $(patsubst %.cpp, ${OBJ_DIR}/%.o, ${SRCS})
DEPS = $(patsubst ${OBJ_DIR}/%.o, ${OBJ_DIR}/%.dep, ${OBJS})

BIN_DIR = $(dir ${TARGET})
OBJS_SUBDIR = $(dir $(firstword ${OBJS}))
	
CC = g++
LD = g++

.PHONY: all clean

all: ${TARGET}

mkdirs:	
	test -d ${BIN_DIR} || mkdir -p ${BIN_DIR}
	test -d ${OBJS_SUBDIR} || mkdir -p ${OBJS_SUBDIR}

${TARGET}: mkdirs ${OBJS}
	${LD} ${LDFLAGS} -o $@ ${OBJS}
	@echo -e "\n<Tips> Build '$(TARGET)' success!\n"

${OBJ_DIR}/%.o: %.cpp
	${CC} ${CFLAGS} ${INCS} -MMD -MP -MF$(@:%.o=%.dep) -MT$@ -o $@ -c $<

clean:
	rm -rf ${TARGET} ${OBJ_DIR}
	@echo -e "\n<Tips> clean complete!\n"

-include ${DEPS}
