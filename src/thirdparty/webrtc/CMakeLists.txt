cmake_minimum_required(VERSION 2.8.11)
project(webrtc)

message("Build webrtc ...")

add_subdirectory(system_wrappers)
add_subdirectory(api)
add_subdirectory(rtc_base)
add_subdirectory(p2p)
add_subdirectory(pc)
add_subdirectory(call)
add_subdirectory(stats)
add_subdirectory(logging)
add_subdirectory(media/base)
add_subdirectory(modules/rtp_rtcp)
add_subdirectory(modules/audio_processing)
add_subdirectory(examples)

