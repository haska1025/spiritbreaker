message("Build rtc_base static library")

file(GLOB_RECURSE rtc_base_srcs *.h *.c *.cc)

list(FILTER  rtc_base_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_base_srcs EXCLUDE REGEX ".*gunit.*")
list(FILTER  rtc_base_srcs EXCLUDE REGEX "mac_.*.cc")
list(FILTER  rtc_base_srcs EXCLUDE REGEX "task_queue_gcd.cc")
list(FILTER  rtc_base_srcs EXCLUDE REGEX "task_queue_libevent.cc")
if(WIN32)
else()
    list(FILTER  rtc_base_srcs EXCLUDE REGEX ".*_win.cc")
    list(FILTER  rtc_base_srcs EXCLUDE REGEX ".*win.*")
endif(WIN32)

source_group(source FILES ${rtc_base_srcs})

add_library(rtc_base OBJECT ${rtc_base_srcs})

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp) 

target_include_directories(rtc_base PRIVATE ..)
target_include_directories(rtc_base PRIVATE ../../abseil-cpp/)
target_include_directories(rtc_base PRIVATE ../../boringssl/src/include/)
target_include_directories(rtc_base PUBLIC ${JSONCPP_INCLUDE_DIRS})

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_base PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_base PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_base PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
    add_definitions(-DWEBRTC_OPUS_SUPPORT_120MS_PTIME=1)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

