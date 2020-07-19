################################
# 1, 时间戳
# 2, 规则
# 3, Makefile

################################
#  规则,
#  目标: 依赖文件 
#  \t命令
#-------------------------------
# 默认目标, 第一个目标
#-------------------------------
# 伪目标
#-------------------------------

################################
# 变量
# $(NAME)
# NAME = VALUE
# A = B
A = $B
B = hello world
# NAME := VALUE
# C := $D
D = hello world
# NAME += VALUE
# C += $D today
# NAME ?= VALUE
C ?= tomorrow

################################
# 预定义变量
# $^ : 依赖列表
# $@ : 目标
# $< : 依赖列表中的第一个依赖文件

# $* $% 


################################
# 环境变量
# CC CFLAGS LDFLAGS
# CXX CPP CXXFLAGS
#
CC := arm-linux-gcc
CFLAGS += -Wall
LDFLAGS += -lm -lz
export CC CFLAGS LDFLAGS


################################
# 重新定义显式规则
# %.o: %.c
# \t$(CC) -c $(CFLAGS) $< -o $@


################################
# 多目录编译
# make -C DIR
# 读取DIR目录下的Makefile


# CC = gcc -Wall


################################

TARGET := test

OBJS := main.o
OBJS += fun1.o fun2.o
	
all1:
	make -C dir

all: $(TARGET)


clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	make -C dir clean


# a.out: main.o fun1.o fun2.o
$(TARGET): $(OBJS)
	gcc $^ -o $@

main.o: main.c header.h
	gcc -c $< -o $@


################################
# 显式规则
# 隐式规则
# %.o: %.c


.PHONY: clean header.h

all0: 
	echo $(A)
	echo $(C)

