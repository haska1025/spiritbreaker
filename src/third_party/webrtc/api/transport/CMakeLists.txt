message("Build rtc_transport static library")

file(GLOB rtc_transport_srcs 
        bitrate_settings.cc
        field_trial_based_config.cc
        goog_cc_factory.cc
        network_types.cc
	)

source_group(source FILES ${rtc_transport_srcs})

add_library(rtc_transport OBJECT ${rtc_transport_srcs})

target_include_directories(rtc_transport PUBLIC ../..)
target_include_directories(rtc_transport PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_transport PUBLIC ../../../boringssl/src/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_transport PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_transport PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_transport PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

