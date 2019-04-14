message("Build rtc_mediabase static library")

file(GLOB rtc_mediabase_srcs 
        codec.cc
        fake_frame_source.cc
        fake_media_engine.cc
        fake_video_capturer.cc
        fake_video_renderer.cc
        h264_profile_level_id.cc
        media_channel.cc
        media_constants.cc
        media_engine.cc
        rid_description.cc
        rtp_data_engine.cc
        rtp_utils.cc
        stream_params.cc
        turn_utils.cc
        video_adapter.cc
        video_broadcaster.cc
        video_capturer.cc
        video_common.cc
        video_source_base.cc
        vp9_profile.cc
	)

source_group(source FILES ${rtc_mediabase_srcs})

add_library(rtc_mediabase ${rtc_mediabase_srcs})

target_include_directories(rtc_mediabase PUBLIC ../..)
target_include_directories(rtc_mediabase PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_mediabase PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_mediabase PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_mediabase PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

