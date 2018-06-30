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

ifeq ($(LICHEE_TOOLCHAIN_PATH),)

ifeq (${ARCH}, arm64)
    GCC_ARCH=aarch64
    LICHEE_TOOLCHAIN_PATH=${LICHEE_ROOT}/out/external-toolchain/gcc-aarch64/
    LICHEE_CROSS_COMPILER=aarch64-linux-gnu
else
    GCC_ARCH=arm
    LICHEE_TOOLCHAIN_PATH=${LICHEE_ROOT}/out/${LICHEE_CHIP}/linux/common/buildroot/external-toolchain/
    LICHEE_CROSS_COMPILER=arm-linux-gnueabi
endif

ifeq (${LICHEE_KERN_VER}, linux-4.4)
    LICHEE_TOOLCHAIN_PATH=${LICHEE_ROOT}/out/gcc-linaro-5.3.1-2016.05/gcc-${GCC_ARCH}/
endif

ifeq (${LICHEE_KERN_VER}, linux-4.9)
    LICHEE_TOOLCHAIN_PATH=${LICHEE_ROOT}/out/gcc-linaro-5.3.1-2016.05/gcc-${GCC_ARCH}/
endif

endif

export CC:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-gcc
export AR:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-ar
export LD:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-ld
export STIRP:=${LICHEE_TOOLCHAIN_PATH}/bin/${LICHEE_CROSS_COMPILER}-strip
export OUT_DIR:=$(PWD)/out/${LICHEE_ARCH}
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
	-@rm -rf ${OUT_DIR}
	
install:all
	-@mkdir -p ${OUT_DIR}
	@for i in ${SUBDIR}; do \
		(make -C $$i install); \
	done

.PHONY:clean all info install

