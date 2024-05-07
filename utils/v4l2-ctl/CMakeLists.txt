#export PATH=$PATH:/mnt/workspace/cy/devkit/cmake-3.29.2/bin
cmake_minimum_required(VERSION 3.5)

project(v4l2-ctl)

add_definitions(-DNO_LIBV4L2)
include_directories(../../include ../common)
add_compile_options(-include ../../android-config.h)

file(GLOB src_files 
    v4l2-ctl.cpp v4l2-ctl.h v4l2-ctl-common.cpp v4l2-ctl-tuner.cpp 
    v4l2-ctl-io.cpp v4l2-ctl-stds.cpp v4l2-ctl-vidcap.cpp v4l2-ctl-vidout.cpp 
    v4l2-ctl-overlay.cpp v4l2-ctl-vbi.cpp v4l2-ctl-selection.cpp v4l2-ctl-misc.cpp 
    v4l2-ctl-streaming.cpp v4l2-ctl-sdr.cpp v4l2-ctl-edid.cpp v4l2-ctl-modes.cpp 
    v4l2-ctl-meta.cpp v4l2-ctl-subdev.cpp
    ../common/v4l2-tpg-core.c
    ../common/v4l2-tpg-colors.c
    ../common/v4l2-info.cpp
    ../common/v4l-stream.c
    ../common/media-info.cpp
    ../common/codec-v4l2-fwht.c
    ../common/codec-fwht.c)

find_library(
        log-lib
        log )

add_executable(v4l2-ctl ${src_files})
target_link_libraries(v4l2-ctl ${log-lib})