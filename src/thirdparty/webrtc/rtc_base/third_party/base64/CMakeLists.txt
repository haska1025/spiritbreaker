message("Build base64 static library")

file(GLOB rtc_base64_srcs 
	base64.cc
	base64.h
	)

source_group(source FILES ${rtc_base64_srcs})

add_library(rtc_base64 OBJECT ${rtc_base64_srcs})

target_include_directories(rtc_base64 PUBLIC ../../..)
target_include_directories(rtc_base64 PUBLIC ../../../abseil-cpp/)

