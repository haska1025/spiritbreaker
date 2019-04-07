message("Build rtc_p2p static library")
set(rtc_p2p_VERSION 2.0.0)


file(GLOB rtc_p2p_srcs 
	udp_port.h
	transport_description_factory.cc
	transport_description_factory.h
	transport_description.cc
	transport_description.h
	transport_factory_interface.h
	transport_info.h
	turn_port.cc
	turn_port.h
	tcp_port.cc
	tcp_port.h
	transport.h
	stun.cc
	stun.h
	stun_port.cc
	stun_port.h
	stun_request.cc
	stun_request.h
	pseudo_tcp.h
	regathering_controller.cc
	regathering_controller.h
	relay_port.cc
	relay_port.h
	packet_transport_internal.cc
	packet_transport_internal.h
	port_allocator.cc
	port_allocator.h
	port.cc
	port.h
	port_interface.cc
	port_interface.h
	pseudo_tcp.cc
	packet_loss_estimator.cc
	packet_loss_estimator.h
	packet_socket_factory.cc
	packet_socket_factory.h
	packet_transport_interface.h
	p2p_constants.cc
	p2p_constants.h
	p2p_transport_channel.cc
	p2p_transport_channel.h
	mdns_message.h
	mock_async_resolver.h
	mock_ice_transport.h
	no_op_dtls_transport.cc
	no_op_dtls_transport.h
	ice_credentials_iterator.h
	ice_transport_internal.cc
	ice_transport_internal.h
	mdns_message.cc
	dtls_transport_internal.cc
	dtls_transport_internal.h
	fake_candidate_pair.h
	fake_dtls_transport.h
	fake_ice_transport.h
	fake_packet_transport.h
	fake_port_allocator.h
	ice_credentials_iterator.cc
	basic_packet_socket_factory.cc
	basic_packet_socket_factory.h
	candidate_pair_interface.h
	dtls_transport.cc
	dtls_transport.h
	async_stun_tcp_socket.h
	basic_async_resolver_factory.cc
	basic_async_resolver_factory.h
	async_stun_tcp_socket.cc
	stun_server.cc
	stun_server.h
	turn_server.cc
	turn_server.h
	relay_server.cc
	relay_server.h
	)

source_group(source FILES ${rtc_p2p_srcs})

add_library(rtc_p2p ${rtc_p2p_srcs}) 

target_include_directories(rtc_p2p PUBLIC ../..)
target_include_directories(rtc_p2p PUBLIC ../../../abseil-cpp/)


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


