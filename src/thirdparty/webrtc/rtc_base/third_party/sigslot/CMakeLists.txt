message("Build sigslot static library")

file(GLOB rtc_sigslot_srcs 
	sigslot.cc
	sigslot.h
	)

source_group(source FILES ${rtc_sigslot_srcs})

add_library(rtc_sigslot OBJECT ${rtc_sigslot_srcs})

target_include_directories(rtc_sigslot PUBLIC ../../..)
target_include_directories(rtc_sigslot PUBLIC ../../../abseil-cpp/)

