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
    Dbbif *bus = new DbbifSlave();

    bus->aclk = &top->clk;
    bus->aresetn = &top->rst;

    bus->awvalid = &top->nvdla_core2dbb_aw_awvalid;
    bus->awready = &top->nvdla_core2dbb_aw_awready;
    bus->awaddr = (uint64_t *) &top->nvdla_core2dbb_aw_awaddr;
    bus->awlen = &top->nvdla_core2dbb_aw_awlen;
    bus->awid = &top->nvdla_core2dbb_aw_awid;

    bus->wvalid = &top->nvdla_core2dbb_w_wvalid;
    bus->wready = &top->nvdla_core2dbb_w_wready;
    bus->wdata = &top->nvdla_core2dbb_w_wdata;
    bus->wlast = &top->nvdla_core2dbb_w_wlast;
    bus->wstrb = &top->nvdla_core2dbb_w_wstrb;

    bus->bvalid = &top->nvdla_core2dbb_b_bvalid;
    bus->bready = &top->nvdla_core2dbb_b_bready;
    bus->bid = &top->nvdla_core2dbb_b_bid;

    bus->arvalid = &top->nvdla_core2dbb_ar_arvalid;
    bus->arready = &top->nvdla_core2dbb_ar_arready;
    bus->araddr = (uint64_t *) &top->nvdla_core2dbb_ar_araddr;
    bus->arlen = &top->nvdla_core2dbb_ar_arlen;
    bus->arid = &top->nvdla_core2dbb_ar_arid;

    bus->rvalid = &top->nvdla_core2dbb_r_rvalid;
    bus->rready = &top->nvdla_core2dbb_r_rready;
    bus->rlast = &top->nvdla_core2dbb_r_rlast;
    bus->rdata = &top->nvdla_core2dbb_r_rdata;
    bus->rid = &top->nvdla_core2dbb_r_rid;

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
