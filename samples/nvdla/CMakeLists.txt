cmake_minimum_required(VERSION 3.8)

# Name of the project
project(nvdla_basic)

if (NOT DEFINED ENV{HWPE_NVDLA_HOME})
    message(FATAL_ERROR "Please define the HWPE_NVDLA_HOME environment variable")
endif ()

if (NOT DEFINED ENV{RENODE_HOME})
    message(FATAL_ERROR "Please define the RENODE_HOME environment variable")
endif ()

set(NVDLA_VERILATOR_FLAGS "-f $ENV{HWPE_NVDLA_HOME}/test/verilator/verilator_nv_small.f")
set(NVDLA_TOP $ENV{HWPE_NVDLA_HOME}/nvdla_hw/outdir/nv_small/vmod/nvdla/top/NV_nvdla.v)
# Verilog file containing the top module to be Verilated
set(VTOP ${NVDLA_TOP})

set(USER_RENODE_DIR $ENV{HWPE_NVDLA_HOME})

# C/C++ source files to be compiled
set(CSOURCES sim_main.cpp)


# Additional compiling, linking or verilating arguments (respectively)
#set(COMP_EXEC_ARGS ...)
# set(COMP_LIB_ARGS ...)
#set(LINK_EXEC_ARGS ...)
#set(LINK_LIB_ARGS ...)
set(VERI_ARGS ${NVDLA_VERILATOR_FLAGS})
# set(VERI_EXEC_ARGS -Wno-WIDTH)
# set(VERI_LIB_ARGS -Wno-WIDTH)

# CMake file doing the hard job
include(../../cmake/configure-and-verilate.cmake)
