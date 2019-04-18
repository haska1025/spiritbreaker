message("Build rtc_audiomixer static library")

add_definitions(-DWEBRTC_APM_DEBUG_DUMP=0)
file(GLOB_RECURSE rtc_audiomixer_srcs 
                 *.h *.c *.cc
)

source_group(source FILES ${rtc_audiomixer_srcs})

list(FILTER  rtc_audiomixer_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_audiomixer_srcs EXCLUDE REGEX ".*mock.*")

add_library(rtc_audiomixer OBJECT ${rtc_audiomixer_srcs})

target_include_directories(rtc_audiomixer PUBLIC ../../)
target_include_directories(rtc_audiomixer PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_audiomixer PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_audiomixer PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_audiomixer PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

