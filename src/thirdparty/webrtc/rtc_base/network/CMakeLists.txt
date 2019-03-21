message("Build experiments static library")

file(GLOB rtc_network_srcs sent_packet.cc sent_packet.h)

source_group(source FILES ${rtc_network_srcs})

add_library(rtc_network OBJECT ${rtc_network_srcs})

target_include_directories(rtc_network PUBLIC ../..)
target_include_directories(rtc_network PUBLIC ../../../abseil-cpp/)
