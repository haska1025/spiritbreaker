message("Build rtc_api static library")

set(rtc_api_VERSION 2.0.0)

add_subdirectory(crypto)
#add_subdirectory(ortc)
#add_subdirectory(audio)
add_subdirectory(audio_codecs)
add_subdirectory(video)
#add_subdirectory(video_codecs)
add_subdirectory(transport)
#add_subdirectory(task_queue)

file(GLOB rtc_api_srcs 
	candidate.cc
    rtc_error.cc
    rtp_parameters.cc
    rtp_headers.cc
    media_transport_interface.cc
    audio_options.cc
	)

source_group(source FILES ${rtc_api_srcs})

add_library(rtc_api ${rtc_api_srcs}
        $<TARGET_OBJECTS:rtc_crypto>
        $<TARGET_OBJECTS:rtc_video>
        $<TARGET_OBJECTS:rtc_transport>
        $<TARGET_OBJECTS:rtc_audiocodecs>
        )

target_include_directories(rtc_api PUBLIC ..)
target_include_directories(rtc_api PUBLIC ../../abseil-cpp/)
target_include_directories(rtc_api PUBLIC ../../boringssl/src/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_api PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_api PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_api PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

