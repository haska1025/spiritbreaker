message("Build system static library")

file(GLOB rtc_system_srcs 
	fallthrough.h
	file_wrapper.cc
	file_wrapper.h
	ignore_warnings.h
	inline.h
	rtc_export.h
	unused.h
	arch.h
	asm_defines.h)

source_group(source FILES ${rtc_system_srcs})

add_library(rtc_system OBJECT ${rtc_system_srcs})

target_include_directories(rtc_system PUBLIC ../..)
target_include_directories(rtc_system PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif()
