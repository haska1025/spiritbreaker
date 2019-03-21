message("Build experiments static library")

file(GLOB rtc_strings_srcs
	json.cc
	json.h
	string_builder.cc
	string_builder.h
	)

source_group(source FILES ${rtc_strings_srcs})

add_library(rtc_strings OBJECT ${rtc_strings_srcs})

target_include_directories(rtc_strings PUBLIC ../)
target_include_directories(rtc_strings PUBLIC ../..)
target_include_directories(rtc_strings PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_strings PUBLIC ../../../jsoncpp/source/include/)

