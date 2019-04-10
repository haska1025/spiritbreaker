message("Build rtc_rtp_rtcp static library")

file(GLOB rtc_rtp_rtcp_srcs 
        include/rtp_rtcp_defines.cc
        source/contributing_sources.cc
        source/dtmf_queue.cc
        source/fec_private_tables_bursty.cc
        source/fec_private_tables_random.cc
        source/fec_test_helper.cc
        source/flexfec_header_reader_writer.cc
        source/flexfec_receiver.cc
        source/flexfec_sender.cc
        source/forward_error_correction.cc
        source/forward_error_correction_internal.cc
        source/packet_loss_stats.cc
        source/playout_delay_oracle.cc
        source/receive_statistics_impl.cc
        source/remote_ntp_time_estimator.cc
        source/rtcp_nack_stats.cc
        source/rtcp_packet.cc
        source/rtcp_receiver.cc
        source/rtcp_sender.cc
        source/rtcp_transceiver.cc
source/rtcp_transceiver_config.cc
source/rtcp_transceiver_impl.cc
source/rtp_generic_frame_descriptor.cc
source/rtp_generic_frame_descriptor_extension.cc
source/rtp_header_extension_map.cc
source/rtp_header_extensions.cc
source/rtp_header_extension_size.cc
source/rtp_header_parser.cc
source/rtp_packet.cc
source/rtp_packet_history.cc
source/rtp_packet_received.cc
source/rtp_packet_to_send.cc
source/rtp_rtcp_impl.cc
source/rtp_sender_audio.cc
source/rtp_sender.cc
source/rtp_sender_video.cc
source/rtp_utility.cc
source/rtp_video_header.cc
source/time_util.cc
source/tmmbr_help.cc
source/ulpfec_generator.cc
source/ulpfec_header_reader_writer.cc
source/ulpfec_receiver_impl.cc
)

source_group(source FILES ${rtc_rtp_rtcp_srcs})

add_library(rtc_rtp_rtcp ${rtc_rtp_rtcp_srcs})

target_include_directories(rtc_rtp_rtcp PUBLIC ../../)
target_include_directories(rtc_rtp_rtcp PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_rtp_rtcp PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_rtp_rtcp PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_rtp_rtcp PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

