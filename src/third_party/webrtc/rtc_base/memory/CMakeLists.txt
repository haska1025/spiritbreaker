message("Build experiments static library")

file(GLOB rtc_memory_srcs 
	aligned_array.h
	aligned_malloc.cc
	aligned_malloc.h
	)

source_group(source FILES ${rtc_memory_srcs})

add_library(rtc_memory OBJECT ${rtc_memory_srcs})

target_include_directories(rtc_memory PUBLIC ../..)
target_include_directories(rtc_memory PUBLIC ../../../abseil-cpp/)

