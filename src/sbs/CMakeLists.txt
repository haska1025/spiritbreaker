message("Build spiritbreaker...")

cmake_minimum_required(VERSION 2.8.11)

file(GLOB sbs_srcs
        command_handler.cpp
        command_handler.h
        configuration.cpp
        configuration.h
        httpbase.cc
        httpbase.h
        httpcommon.cc
        httpcommon.h
        httpserver.cc
        httpserver.h
        memory_stream.cpp
        memory_stream.h
        peer.cpp
        peer.h
        publisher.cpp
        publisher.h
        room.cpp
        room.h
        room_mgr.cpp
        room_mgr.h
        sbs_decl.h
        sbs_error.h
        sbs_log.cpp
        sbs_log.h
        sbs_message.h
        sbs_mgr.cpp
        sbs_mgr.h
        subscriber.cpp
        subscriber.h
        webrtcconnection.cpp
        webrtcconnection.h
)

source_group(source FILES ${sbs_srcs})


link_directories(../thirdparty/boringssl/build/ssl)
link_directories(../thirdparty/boringssl/build/crypto)
link_directories(../thirdparty/libsrtp/)
link_directories(../thirdparty/libjpeg/build)
link_directories(../thirdparty/libyuv/build)

link_directories(../thirdparty/abseil-cpp/build/absl/strings)
link_directories(../thirdparty/abseil-cpp/build/absl/base)
link_directories(../thirdparty/abseil-cpp/build/absl/types/)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

add_library(sbs ${sbs_srcs})

add_executable(spiritbreaker sbs_main.cpp ${sbs_srcs})

target_include_directories(sbs PUBLIC ../thirdparty/webrtc/)
target_include_directories(sbs PUBLIC ../thirdparty/abseil-cpp/)
target_include_directories(sbs PUBLIC ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(spiritbreaker sbs 
        rtc_pc rtc_p2p rtc_call rtc_rtp_rtcp rtc_mediabase rtc_audio_processing rtc_stats rtc_api rtc_logging rtc_base rtc_systemwrappers
        yuv jpeg
        srtp2 ssl crypto 
        absl_bad_optional_access absl_strings absl_base absl_throw_delegate
        ${JSONCPP_LIBRARIES})

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(sbs PUBLIC "-pthread")
    endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(sbs "${CMAKE_THREAD_LIBS_INIT}")
endif()

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(spiritbreaker PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(spiritbreaker PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(spiritbreaker PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

