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

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_base PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_base PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_base PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

