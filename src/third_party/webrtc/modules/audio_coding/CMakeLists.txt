message("Build rtc_audiocoding static library")

file(GLOB rtc_audiocoding_srcs 
        include/*.h
                 acm2/*.h acm2/*.cc acm2/*.c 
                 audio_network_adaptor/*.h audio_network_adaptor/*.cc audio_network_adaptor/*.c
                 codecs/*.h codecs/*.cc
                 codecs/*/*.h codecs/*/*.cc codecs/*/*.c
                 neteq/*.h neteq/*.cc neteq/*.c
                 )

list(FILTER  rtc_audiocoding_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_audiocoding_srcs EXCLUDE REGEX ".*fake.*")
list(FILTER  rtc_audiocoding_srcs EXCLUDE REGEX ".*opus.*")

source_group(source FILES ${rtc_audiocoding_srcs})

add_library(rtc_audiocoding OBJECT ${rtc_audiocoding_srcs})

target_include_directories(rtc_audiocoding PUBLIC ../../)
target_include_directories(rtc_audiocoding PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_audiocoding PUBLIC ../../../opus/)

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

