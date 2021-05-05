//
// Copyright (c) 2010-2021 Antmicro
//
// This file is licensed under the MIT License.
// Full license text is available in 'licenses/MIT.txt'.
//
#include "wishbone.h"

void Wishbone::tick(bool countEnable, uint64_t steps = 1)
{
    for(uint32_t i = 0; i < steps; i++) {
        *wb_clk = 1;
        evaluateModel();
        *wb_clk = 0;
        evaluateModel();
    }

    if(countEnable) {
        tickCounter += steps;
    }
}

void Wishbone::timeoutTick(bool condition, int timeout = 20)
{
    do {
        tick(true);
        timeout--;
    }
    while(condition && timeout > 0);

    if(timeout < 0) {
        throw "Operation timeout";
    }
}

void Wishbone::write(uint64_t addr, uint64_t value)
{
    *wb_we = 1;
    *wb_sel = 0xF;
    *wb_cyc = 1;
    *wb_stb = 1;
//  According to Wishbone B4 spec when using 32 bit bus with byte granularity
//  we drop 2 youngest bits
    *wb_addr = addr >> 2;
    *wb_wr_dat = value;

    timeoutTick(*wb_ack == 1);
    tick(true);

    *wb_stb = 0;
    *wb_cyc = 0;
    *wb_we = 0;
    *wb_sel = 0;

    timeoutTick(*wb_ack == 0);
    tick(true);
}

uint64_t Wishbone::read(uint64_t addr)
{
    *wb_we = 0;
    *wb_sel = 0xF;
    *wb_cyc = 1;
    *wb_stb = 1;
    *wb_addr = addr >> 2;

    timeoutTick(*wb_ack == 1);
    tick(true);
    uint64_t result = *wb_rd_dat;

    *wb_cyc = 0;
    *wb_stb = 0;
    *wb_sel = 0;

    timeoutTick(*wb_ack == 0);
    tick(true);

    return result;
}

void Wishbone::reset()
{
    *wb_rst = 1;
    tick(true);
    *wb_rst = 0;
    tick(true);
}
