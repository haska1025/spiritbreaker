message("Build stun probe...")

file(GLOB stun_probe_src *.cc *.h)

link_directories(../../../boringssl/src/build/ssl)
link_directories(../../../boringssl/src/build/crypto)
link_directories(../../../abseil-cpp/build/absl/strings)
link_directories(../../../abseil-cpp/build/absl/base)
link_directories(../../../abseil-cpp/build/absl/types/)

source_group(source FILES ${stun_probe_src})
add_executable(stunprobe ${stun_probe_src})

target_include_directories(stunprobe PUBLIC ../../)
target_include_directories(stunprobe PUBLIC ../../../abseil-cpp/)
target_link_libraries(stunprobe webrtc ssl crypto
        absl_strings absl_base absl_internal_throw_delegate 
        )


find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
	target_compile_options(stunprobe PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
	target_link_libraries(stunprobe "${CMAKE_THREAD_LIBS_INIT}")
endif()


if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(stunprobe PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(stunprobe PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(stunprobe PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)


