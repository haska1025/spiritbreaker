message("Build rtc_media static library")

add_definitions(-DHAVE_WEBRTC_VIDEO)
add_definitions(-DHAVE_WEBRTC_VOICE)
add_definitions(-DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE)

file(GLOB_RECURSE rtc_media_srcs *.h *.cc *.c)

list(FILTER  rtc_media_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_media_srcs EXCLUDE REGEX ".*fake.*")
#list(FILTER  rtc_media_srcs EXCLUDE REGEX ".*sctp.*")

source_group(media FILES ${rtc_media_srcs})

add_library(rtc_media OBJECT ${rtc_media_srcs})

target_include_directories(rtc_media PUBLIC ../)
target_include_directories(rtc_media PUBLIC ../../)
target_include_directories(rtc_media PUBLIC ../../abseil-cpp/)
target_include_directories(rtc_media PUBLIC ../../usrsctp)
target_include_directories(rtc_media PUBLIC ../../libyuv/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_media PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_media PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_media PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

