message("Build experiments static library")

file(GLOB rtc_strings_srcs
	json.cc
	json.h
	string_builder.cc
	string_builder.h
	)

source_group(source FILES ${rtc_strings_srcs})

add_library(rtc_strings OBJECT ${rtc_strings_srcs})

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp) 

target_include_directories(rtc_strings PUBLIC ../)
target_include_directories(rtc_strings PUBLIC ../..)
target_include_directories(rtc_strings PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_strings PUBLIC ${JSONCPP_INCLUDE_DIRS})

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_strings PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_strings PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

