cmake_minimum_required(VERSION 2.8.11)
project(webrtc)

message("Build webrtc ...")
set (CMAKE_POSITION_INDEPENDENT_CODE TRUE) 
add_subdirectory(system_wrappers)
add_subdirectory(api)
add_subdirectory(rtc_base)
add_subdirectory(p2p)
add_subdirectory(pc)
add_subdirectory(call)
add_subdirectory(stats)
add_subdirectory(logging)
add_subdirectory(media)
add_subdirectory(audio)
add_subdirectory(video)
add_subdirectory(common_audio)
add_subdirectory(common_video)
add_subdirectory(modules)
add_subdirectory(modules/rtp_rtcp)
add_subdirectory(modules/audio_processing)
add_subdirectory(modules/third_party)
add_subdirectory(modules/audio_coding)
add_subdirectory(modules/video_coding)
add_subdirectory(modules/congestion_controller)
add_subdirectory(modules/bitrate_controller)
add_subdirectory(modules/pacing)
add_subdirectory(modules/remote_bitrate_estimator)
add_subdirectory(modules/utility)
add_subdirectory(modules/audio_mixer)
add_subdirectory(modules/audio_device)
#add_subdirectory(examples)

add_library(webrtc STATIC 
	$<TARGET_OBJECTS:rtc_systemwrappers> 
	$<TARGET_OBJECTS:rtc_base> 
	$<TARGET_OBJECTS:rtc_api> 
	$<TARGET_OBJECTS:rtc_pc> 
	$<TARGET_OBJECTS:rtc_p2p> 
	$<TARGET_OBJECTS:rtc_logging> 
	$<TARGET_OBJECTS:rtc_audio> 
	$<TARGET_OBJECTS:rtc_video> 
	$<TARGET_OBJECTS:rtc_audiothird> 
	$<TARGET_OBJECTS:rtc_audiocoding> 
	$<TARGET_OBJECTS:rtc_audioprocessing> 
	$<TARGET_OBJECTS:rtc_videocoding> 
	$<TARGET_OBJECTS:rtc_media> 
	$<TARGET_OBJECTS:rtc_stats> 
	$<TARGET_OBJECTS:rtc_call> 
	$<TARGET_OBJECTS:rtc_rtprtcp> 
	$<TARGET_OBJECTS:rtc_pacing> 
	$<TARGET_OBJECTS:rtc_commonaudio> 
	$<TARGET_OBJECTS:rtc_commonvideo> 
	$<TARGET_OBJECTS:rtc_congestioncontroller> 
	$<TARGET_OBJECTS:rtc_bitratecontroller> 
	$<TARGET_OBJECTS:rtc_remotebitrateestimator> 
	$<TARGET_OBJECTS:rtc_utility> 
	$<TARGET_OBJECTS:rtc_modulescommon> 
	$<TARGET_OBJECTS:rtc_audiomixer> 
	$<TARGET_OBJECTS:rtc_audiodevice> 
    )

#set_property(TARGET webrtc PROPERTY POSITION_INDEPENDENT_CODE ON)

#add_custom_command(OUTPUT lib${PROJECT_NAME}.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/system_wrappers/librtc_systemwrappers.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/rtc_base/librtc_base.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/logging/librtc_logging.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/modules/third_party/librtc_audiothird.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/modules/audio_coding/librtc_audiocoding.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/modules/audio_processing/librtc_audio_processing.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/api/librtc_api.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/media/librtc_media.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/stats/librtc_stats.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/modules/rtp_rtcp/librtc_rtp_rtcp.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/call/librtc_call.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/p2p/librtc_p2p.a
#        COMMAND ar ARGS -x ${CMAKE_CURRENT_BINARY_DIR}/pc/librtc_pc.a
#        COMMAND ar ARGS -r lib${PROJECT_NAME}.a *.o
#        COMMAND rm ARGS -f *.o
#        )
#
#add_custom_target(${PROJECT_NAME} ALL DEPENDS lib${PROJECT_NAME}.a)
