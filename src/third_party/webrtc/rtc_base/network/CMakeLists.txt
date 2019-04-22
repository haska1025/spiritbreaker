message("Build experiments static library")

file(GLOB rtc_network_srcs sent_packet.cc sent_packet.h)

source_group(source FILES ${rtc_network_srcs})

add_library(rtc_network OBJECT ${rtc_network_srcs})

target_include_directories(rtc_network PUBLIC ../..)
target_include_directories(rtc_network PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(tm PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	if(NOT LOG_TRANS_DEFINED)
		# Find the LogDll library
		find_library(LOG_LIBRARY LogDll)
	endif(NOT LOG_TRANS_DEFINED)
	# require min win system version xp (0x0501)
	target_compile_definitions(tm PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(tm PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

