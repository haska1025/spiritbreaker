message("Build rtc_video static library")

file(GLOB_RECURSE rtc_video_srcs *.h *.cc *.c)

source_group(source FILES ${rtc_video_srcs})

list(FILTER  rtc_video_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_video_srcs EXCLUDE REGEX ".*mock.*")
list(FILTER  rtc_video_srcs EXCLUDE REGEX ".*mips.*")
list(FILTER  rtc_video_srcs EXCLUDE REGEX ".*arm.*")
list(FILTER  rtc_video_srcs EXCLUDE REGEX ".*loopback.*")
list(FILTER  rtc_video_srcs EXCLUDE REGEX "video_analyzer.cc")
list(FILTER  rtc_video_srcs EXCLUDE REGEX "video_replay.cc")

add_library(rtc_video OBJECT ${rtc_video_srcs})

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp) 

target_include_directories(rtc_video PUBLIC ../../)
target_include_directories(rtc_video PUBLIC ../)
target_include_directories(rtc_video PUBLIC ../../abseil-cpp/)
target_include_directories(rtc_video PUBLIC ${JSONCPP_INCLUDE_DIRS})

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_video PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_video PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_video PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
    add_definitions(-DWEBRTC_APM_DEBUG_DUMP=0)
    add_definitions(-DWEBRTC_NS_FLOAT)
endif(WIN32)

