# top makefile for disptest
# author:tracyone@live.cn
#
SUBDIR := framebuffer
LICHEE_ROOT := $(PWD)/..
BUIDCONFIG := ${LICHEE_ROOT}/.buildconfig
ifneq ("$(wildcard ${BUIDCONFIG})","")
include ${BUIDCONFIG}
else
$(error "${BUIDCONFIG} is not exist!! Please config lichee first.")
endif

export CC:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-gcc
export AR:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-ar
export LD:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-ld
export STIRP:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-strip
export OUTDIR:=$(PWD)/out
export CFLAGS+=-I$(PWD)/include



all:info
	@for i in ${SUBDIR}; do \
		(make -C $$i); \
	done

info:
	@echo "================================================================"
	@echo Platform	:${LICHEE_PLATFORM}
	@echo Chip	:${LICHEE_CHIP}
	@echo Kernel	:${LICHEE_KERN_VER}
	@echo ARCH	:${LICHEE_ARCH}
	@echo Board	:${LICHEE_BOARD}
	@echo CC prefix	:${LICHEE_CROSS_COMPILER}
	@echo toolchain	:${LICHEE_TOOLCHAIN_PATH}
	@echo "================================================================"


clean:
	@for i in ${SUBDIR}; do \
		(make -C $$i clean); \
	done
	-@rm -rf ${OUTDIR}
	
install:all
	-@mkdir -p ${OUTDIR}
	@for i in ${SUBDIR}; do \
		(make -C $$i install); \
	done

.PHONY:clean all info install

