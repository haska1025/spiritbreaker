message("Build rtc_video static library")

file(GLOB rtc_video_srcs 
        color_space.cc
        encoded_frame.cc
        encoded_image.cc
        hdr_metadata.cc
        video_content_type.cc
	)

source_group(source FILES ${rtc_video_srcs})

add_library(rtc_video OBJECT ${rtc_video_srcs})

target_include_directories(rtc_video PUBLIC ../..)
target_include_directories(rtc_video PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_video PUBLIC ../../../boringssl/src/include/)

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
endif(WIN32)

