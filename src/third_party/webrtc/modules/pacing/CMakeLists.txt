message("Build rtc_pacing static library")

file(GLOB_RECURSE rtc_pacing_srcs 
                 *.h *.c *.cc
)

source_group(source FILES ${rtc_pacing_srcs})

list(FILTER  rtc_pacing_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_pacing_srcs EXCLUDE REGEX ".*mock.*")

add_library(rtc_pacing OBJECT ${rtc_pacing_srcs})

target_include_directories(rtc_pacing PUBLIC ../../)
target_include_directories(rtc_pacing PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_pacing PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_pacing PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_pacing PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

