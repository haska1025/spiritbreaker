message("Build time static library")

file(GLOB rtc_time_srcs 
	timestamp_extrapolator.cc
	timestamp_extrapolator.h
	)

source_group(source FILES ${rtc_time_srcs})

add_library(rtc_time OBJECT ${rtc_time_srcs})

target_include_directories(rtc_time PUBLIC ../..)
target_include_directories(rtc_time PUBLIC ../../../abseil-cpp/)

