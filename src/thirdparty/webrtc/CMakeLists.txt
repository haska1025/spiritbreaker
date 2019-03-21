cmake_minimum_required(VERSION 2.8.11)
project(webrtc)

message("Build webrtc ...")

add_subdirectory(system_wrappers)
add_subdirectory(api)
add_subdirectory(rtc_base)
add_subdirectory(p2p)
add_subdirectory(examples)

