message("Build rtc_audiothird static library")

file(GLOB_RECURSE rtc_audiothird_srcs . *.h *.c *.cc)

list(FILTER  rtc_audiothird_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_audiothird_srcs EXCLUDE REGEX ".*fake.*")
list(FILTER  rtc_audiothird_srcs EXCLUDE REGEX ".*opus.*")

source_group(source FILES ${rtc_audiothird_srcs})

add_library(rtc_audiothird OBJECT ${rtc_audiothird_srcs})

target_include_directories(rtc_audiothird PUBLIC ../../)
target_include_directories(rtc_audiothird PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_audiothird PUBLIC ../../../opus/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_audiothird PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_audiothird PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_audiothird PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

