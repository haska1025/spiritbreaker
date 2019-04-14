message("Build rtc_p2p static library")


file(GLOB rtc_p2p_srcs 
	base/transport_description_factory.cc
	base/transport_description.cc
	base/turn_port.cc
	base/tcp_port.cc
	base/stun.cc
	base/stun_port.cc
	base/stun_request.cc
	base/regathering_controller.cc
	base/relay_port.cc
	base/packet_transport_internal.cc
	base/port_allocator.cc
	base/port.cc
	base/port_interface.cc
	base/pseudo_tcp.cc
	base/packet_loss_estimator.cc
	base/packet_socket_factory.cc
	base/p2p_constants.cc
	base/p2p_transport_channel.cc
	base/no_op_dtls_transport.cc
	base/ice_transport_internal.cc
	base/mdns_message.cc
	base/dtls_transport_internal.cc
	base/ice_credentials_iterator.cc
	base/basic_packet_socket_factory.cc
	base/dtls_transport.cc
	base/basic_async_resolver_factory.cc
	base/async_stun_tcp_socket.cc
	base/stun_server.cc
	base/turn_server.cc
	base/relay_server.cc
    client/basic_port_allocator.cc
    client/turn_port_factory.cc
	)

source_group(source FILES ${rtc_p2p_srcs})

add_library(rtc_p2p ${rtc_p2p_srcs}) 

target_include_directories(rtc_p2p PUBLIC ..)
target_include_directories(rtc_p2p PUBLIC ../../abseil-cpp/)


if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_p2p PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_p2p PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_p2p PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)


