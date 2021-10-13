//
// Copyright (c) 2021 Antmicro
//
//  This file is licensed under the MIT License.
//  Full license text is available in 'LICENSE' file.
//
#include <verilated.h>
#include "VNvdla.h"
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if VM_TRACE
# include <verilated_vcd_c.h>
#endif

#include "src/renode_cfu.h"
#include "src/buses/nvdla-slave.h"

RenodeAgent *nvdla;
VNvdla *top = new VNvdla;
VerilatedVcdC *tfp;
vluint64_t main_time = 0;

void eval() {
#if VM_TRACE
    main_time++;
    tfp->dump(main_time);
    tfp->flush();
#endif
    top->eval();
}

RenodeAgent *Init() {
    Dbbif *bus = new Dbbif();

    bus->aclk = &top->clk;
    bus->aresetn = &top->rst;

    bus->awvalid = &top->awvalid;
    bus->awready = &top->awready;
    bus->awaddr = (uint64_t *) &top->awaddr;
    bus->awlen = &top->awlen;
    bus->awid = &top->awid;

    bus->wvalid = &top->wvalid;
    bus->wready = &top->wready;
    bus->wdata = &top->wdata;
    bus->wlast = &top->wlast;
    bus->wstrb = &top->wstrb;

    bus->bvalid = &top->bvalid;
    bus->bready = &top->bready;
    bus->bid = &top->bid;

    bus->arvalid = &top->arvalid;
    bus->arready = &top->arready;
    bus->araddr = (uint64_t *) &top->araddr;
    bus->arlen = &top->arlen;
    bus->arid = &top->arid;

    bus->rvalid = &top->rvalid;
    bus->rready = &top->rready;
    bus->rlast = &top->rlast;
    bus->rdata = &top->rdata;
    bus->rid = &top->rid;

    bus->evaluateModel = &eval;

    nvdla = new RenodeAgent(bus);

#if VM_TRACE
    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("simx.vcd");
#endif

    return nvdla;
}
