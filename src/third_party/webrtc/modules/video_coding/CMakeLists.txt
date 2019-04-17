message("Build rtc_videocoding static library")

file(GLOB_RECURSE rtc_videocoding_srcs *.c *.h *.cc)

list(FILTER  rtc_videocoding_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_videocoding_srcs EXCLUDE REGEX ".*fake.*")
list(FILTER  rtc_videocoding_srcs EXCLUDE REGEX ".*opus.*")

source_group(source FILES ${rtc_videocoding_srcs})

add_library(rtc_videocoding OBJECT ${rtc_videocoding_srcs})

target_include_directories(rtc_videocoding PUBLIC ../../)
target_include_directories(rtc_videocoding PUBLIC ../../..)
target_include_directories(rtc_videocoding PUBLIC ../../../../)
target_include_directories(rtc_videocoding PUBLIC ../../../ffmpeg/)
target_include_directories(rtc_videocoding PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_videocoding PUBLIC ../../../libyuv/include/)
target_include_directories(rtc_videocoding PUBLIC ../../../libvpx/source/libvpx/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_videocoding PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_videocoding PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_videocoding PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

