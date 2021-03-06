message("Build rtc_audio static library")

file(GLOB_RECURSE rtc_audio_srcs *.h *.cc *.c)

source_group(source FILES ${rtc_audio_srcs})

list(FILTER  rtc_audio_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_audio_srcs EXCLUDE REGEX ".*mock.*")
list(FILTER  rtc_audio_srcs EXCLUDE REGEX ".*mips.*")
list(FILTER  rtc_audio_srcs EXCLUDE REGEX ".*arm.*")
list(FILTER  rtc_audio_srcs EXCLUDE REGEX ".*neon.*")
list(FILTER  rtc_audio_srcs EXCLUDE REGEX ".*dump.*")
list(FILTER  rtc_audio_srcs EXCLUDE REGEX "fixed_gain_controller.cc")

add_library(rtc_audio OBJECT ${rtc_audio_srcs})

target_include_directories(rtc_audio PUBLIC ../../)
target_include_directories(rtc_audio PUBLIC ../)
target_include_directories(rtc_audio PUBLIC ../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_audio PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_audio PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_audio PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
    add_definitions(-DWEBRTC_APM_DEBUG_DUMP=0)
    add_definitions(-DWEBRTC_NS_FLOAT)
endif(WIN32)

