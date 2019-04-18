message("Build rtc_audiodevice static library")

add_definitions(-DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE)
file(GLOB_RECURSE rtc_audiodevice_srcs *.h *.c *.cc)

source_group(source FILES ${rtc_audiodevice_srcs})

list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX ".*mock.*")
list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX ".*mac.*")
list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX ".*ios.*")
list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX ".*win.*")
list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX ".*android.*")
list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX "audio_device_factory.cc")
#list(FILTER  rtc_audiodevice_srcs EXCLUDE REGEX ".*dummy.*")

add_library(rtc_audiodevice OBJECT ${rtc_audiodevice_srcs})

target_include_directories(rtc_audiodevice PUBLIC ../../)
target_include_directories(rtc_audiodevice PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_audiodevice PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_audiodevice PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_audiodevice PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
    add_definitions(-DLINUX_ALSA)
endif(WIN32)

