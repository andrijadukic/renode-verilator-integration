//
// Copyright (c) 2010-2021 Antmicro
//
// This file is licensed under the MIT License.
// Full license text is available in 'licenses/MIT.txt'.
//
#include "axi.h"
#include <cmath>

Axi::Axi(uint32_t dataWidth, uint32_t addrWidth)
{
    if(dataWidth != 32)
        throw "Unsupported AXI data width";

    this->dataWidth = dataWidth;

    if(addrWidth != 32)
        throw "Unsupported AXI address width";

    this->addrWidth = addrWidth;
}

void Axi::setAgent(RenodeAgent* agent)
{
    this->agent = agent;
}

void Axi::tick(bool countEnable, uint64_t steps = 1)
{
    for(uint64_t i = 0; i < steps; i++) {
        *aclk = 1;
        evaluateModel();
        *aclk = 0;
        evaluateModel();
    }

    if(countEnable) {
        tickCounter += steps;
    }
}

void Axi::timeoutTick(uint8_t *signal, uint8_t value, int timeout)
{
    do {
        tick(true);
        timeout--;
    } while ((*signal != value) && timeout > 0);

    if (timeout == 0) {
        throw "Operation timeout";
    }
}

void Axi::write(uint64_t addr, uint64_t value)
{
    *awvalid = 1;
    *awlen   = 0; // TODO: Variable write length
    *awsize  = 2; // TODO: Variable write width
    *awburst = static_cast<uint8_t>(AxiBurstType::INCR);
    *awaddr  = addr;

    this->agent->log(0, "Axi write - AW");

    timeoutTick(awready, 1);
    tick(true);
    *awvalid = 0;
    tick(true);

    this->agent->log(0, "Axi write - W");

    *wvalid = 1;
    *wdata = value;
    *wstrb = 0xF; // TODO: Byte selects
    *wlast = 1; // TODO: Variable write length

    timeoutTick(wready, 1);
    tick(true);
    *wvalid = 0;
    tick(true);

    this->agent->log(0, "Axi write - B");

    *bready = 1;

    timeoutTick(bvalid, 1);
    tick(true);
    *bready = 0;
    tick(true);
}

uint64_t Axi::read(uint64_t addr)
{
    uint64_t result;

    *arvalid = 1;
    *arlen   = 0; // TODO: Variable read length
    *arsize  = 2; // TODO: Variable read width
    *arburst = static_cast<uint8_t>(AxiBurstType::INCR);
    *araddr  = addr;

    this->agent->log(0, "Axi read - AR");

    timeoutTick(arready, 1);
    tick(true);
    *arvalid = 0;
    tick(true);

    this->agent->log(0, "Axi read - R");

    *rready = 1;

    timeoutTick(rvalid, 1);
    tick(true);
    result = *rdata;
    *rready = 0;
    tick(true);

    return result;
}

void Axi::reset()
{
    *aresetn = 1;
    tick(true);
    *aresetn = 0;
    tick(true);
}
