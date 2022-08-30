/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     trap_c.c
 * @brief    source file for the trap process
 * @version  V1.0
 * @date     12. December 2017
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <core_802.h>
#include "rom_sym_def.h"
#include "log.h"


void (*trap_c_callback)(void);


void trap_c(uint32_t *regs)
{
    int i;
    uint32_t vec = (__get_PSR() & PSR_VEC_Msk) >> PSR_VEC_Pos;

    LOG_ERROR("CPU Exception: NO.%u\n", vec);

    for (i = 0; i < 16; i++) {
        LOG_ERROR("r%d: %08x\t", i, regs[i]);

        if ((i % 4) == 3) {
            LOG_ERROR("\n");
        }
    }

    LOG_ERROR("\n");
    LOG_ERROR("epsr: %8x\n", regs[16]);
    LOG_ERROR("epc : %8x\n", regs[17]);

    if (trap_c_callback) {
        trap_c_callback();
    }

    while (1);
}

