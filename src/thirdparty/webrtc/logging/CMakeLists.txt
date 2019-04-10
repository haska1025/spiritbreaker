message("Build rtc_logging static library")

file(GLOB rtc_logging_srcs 
    rtc_event_log/fake_rtc_event_log.cc
    rtc_event_log/fake_rtc_event_log_factory.cc
    rtc_event_log/ice_logger.cc
    rtc_event_log/logged_events.cc
    rtc_event_log/rtc_event_log.cc
    rtc_event_log/rtc_event_log_factory.cc
    rtc_event_log/rtc_event_log_impl.cc
    rtc_event_log/rtc_event_log_parser.cc
    rtc_event_log/rtc_event_processor.cc
    rtc_event_log/rtc_stream_config.cc
    rtc_event_log/events/rtc_event_alr_state.cc
    rtc_event_log/events/rtc_event_audio_network_adaptation.cc
    rtc_event_log/events/rtc_event_audio_playout.cc
    rtc_event_log/events/rtc_event_audio_receive_stream_config.cc
    rtc_event_log/events/rtc_event_audio_send_stream_config.cc
    rtc_event_log/events/rtc_event_bwe_update_delay_based.cc
    rtc_event_log/events/rtc_event_bwe_update_loss_based.cc
    rtc_event_log/events/rtc_event_dtls_transport_state.cc
    rtc_event_log/events/rtc_event_dtls_writable_state.cc
    rtc_event_log/events/rtc_event_ice_candidate_pair.cc
    rtc_event_log/events/rtc_event_ice_candidate_pair_config.cc
    rtc_event_log/events/rtc_event_probe_cluster_created.cc
    rtc_event_log/events/rtc_event_probe_result_failure.cc
    rtc_event_log/events/rtc_event_probe_result_success.cc
    rtc_event_log/events/rtc_event_rtcp_packet_incoming.cc
    rtc_event_log/events/rtc_event_rtcp_packet_outgoing.cc
    rtc_event_log/events/rtc_event_rtp_packet_incoming.cc
    rtc_event_log/events/rtc_event_rtp_packet_outgoing.cc
    rtc_event_log/events/rtc_event_video_receive_stream_config.cc
    rtc_event_log/events/rtc_event_video_send_stream_config.cc
	)

source_group(source FILES ${rtc_logging_srcs})


find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()

message("protobuf path: ${PROTOBUF_INCLUDE_DIRS}")
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
        rtc_event_log/rtc_event_log.proto
        rtc_event_log/rtc_event_log2.proto
        )

add_library(rtc_logging ${rtc_logging_srcs} ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(rtc_logging PUBLIC ../)
target_include_directories(rtc_logging PUBLIC ../../abseil-cpp/)

add_definitions(-DWEBRTC_ENABLE_PROTOBUF)
if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_logging PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_logging PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_logging PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

