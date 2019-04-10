message("Build rtc_pc static library")

cmake_minimum_required(VERSION 2.8.11)
set(rtc_pc_VERSION 2.0.0)


file(GLOB rtc_pc_srcs 
    webrtc_sdp.cc
    transport_stats.cc
    track_media_info_map.cc
    stats_collector.cc
    simulcast_description.cc
    srtp_filter.cc
    srtp_session.cc
    srtp_transport.cc
    sdp_serializer.cc
    sdp_utils.cc
    session_description.cc
    rtp_transceiver.cc
    rtp_transport.cc
    sctp_utils.cc
    rtp_parameters_conversion.cc
    rtp_receiver.cc
    rtp_sender.cc
    rtcp_mux_filter.cc
    rtc_stats_traversal.cc
    rtp_media_utils.cc
    remote_audio_source.cc
    rtc_stats_collector.cc
    media_stream.cc
    media_stream_observer.cc
    local_audio_source.cc
    media_session.cc
    jsep_session_description.cc
    jsep_transport.cc
    jsep_transport_controller.cc
    dtmf_sender.cc
    external_hmac.cc
    ice_server_parsing.cc
    jsep_ice_candidate.cc
    dtls_srtp_transport.cc
    dtls_transport.cc
    channel_manager.cc
    channel.cc
    )

source_group(source FILES ${rtc_pc_srcs})

add_library(rtc_pc ${rtc_pc_srcs} )

target_include_directories(rtc_pc PUBLIC ..)
target_include_directories(rtc_pc PUBLIC ../../libsrtp/include)
target_include_directories(rtc_pc PUBLIC ../../libsrtp/config)
target_include_directories(rtc_pc PUBLIC ../../libsrtp/crypto/include)
target_include_directories(rtc_pc PUBLIC ../../abseil-cpp/)
target_include_directories(rtc_pc PUBLIC ../../boringssl/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_pc PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_pc PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_pc PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

