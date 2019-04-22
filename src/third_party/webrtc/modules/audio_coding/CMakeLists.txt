message("Build rtc_audiocoding static library")


add_definitions(-DWEBRTC_USE_BUILTIN_ISAC_FLOAT)

file(GLOB_RECURSE rtc_audiocoding_srcs *.h *.c *.cc)

list(FILTER  rtc_audiocoding_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_audiocoding_srcs EXCLUDE REGEX ".*fake.*")
list(FILTER  rtc_audiocoding_srcs EXCLUDE REGEX ".*mips.*")
list(FILTER  rtc_audiocoding_srcs EXCLUDE REGEX ".*neon.*")

source_group(source FILES ${rtc_audiocoding_srcs})

add_library(rtc_audiocoding OBJECT ${rtc_audiocoding_srcs})
include_directories(${CMAKE_BINARY_DIR}/src/third_party/webrtc/logging/)

target_include_directories(rtc_audiocoding PUBLIC ../../)
target_include_directories(rtc_audiocoding PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_audiocoding PUBLIC ../../../opus/)
target_include_directories(rtc_audiocoding PUBLIC ../../../opus/src/include)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_audiocoding PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_audiocoding PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_audiocoding PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

