/*
 * debug/debug.h
 *
 * Copyright (c) 2007-2018 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#define DISPTEST_DEBUG_LEVEL 0

#define loge(fmt, ...)                                                     \
	printf("[disptest] %s:%d " fmt "", __func__, __LINE__, ##__VA_ARGS__)


#if DISPTEST_DEBUG_LEVEL >= 1
#define logd(fmt, ...)                                                     \
	printf("[disptest] %s:%d " fmt "", __func__, __LINE__, ##__VA_ARGS__)
#else
#define logd(fmt, ...)
#endif /*endif EDP_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /*End of file*/
