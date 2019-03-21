message("Build experiments static library")

file(GLOB rtc_synchronize_srcs
	rw_lock_wrapper.cc
	rw_lock_wrapper.h)


if(NOT WIN32)
	file(GLOB rtc_synchronize_srcs ${rtc_synchronize_srcs}
	rw_lock_posix.cc
	rw_lock_posix.h)
else()
	file(GLOB rtc_synchronize_srcs ${rtc_synchronize_srcs}
	rw_lock_win.cc
	rw_lock_win.h)
endif(NOT WIN32)

source_group(source FILES ${rtc_synchronize_srcs})

add_library(rtc_synchronize OBJECT ${rtc_synchronize_srcs})

target_include_directories(rtc_synchronize PUBLIC ../..)
target_include_directories(rtc_synchronize PUBLIC ../../../abseil-cpp/)

