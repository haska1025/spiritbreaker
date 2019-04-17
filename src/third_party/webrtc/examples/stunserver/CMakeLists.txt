message("Build stun server...")

file(GLOB stun_server_src stunserver_main.cc)

link_directories(../../../boringssl/build/ssl)
link_directories(../../../boringssl/build/crypto)

source_group(source FILES ${stun_server_src})
add_executable(stunserver ${stun_server_src})
target_link_libraries(stunserver rtc_p2p rtc_base ssl crypto)


find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
	target_compile_options(stunserver PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
	target_link_libraries(stunserver "${CMAKE_THREAD_LIBS_INIT}")
endif()


if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(tm PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	if(NOT LOG_TRANS_DEFINED)
		# Find the LogDll library
		find_library(LOG_LIBRARY LogDll)
	endif(NOT LOG_TRANS_DEFINED)
	# require min win system version xp (0x0501)
	target_compile_definitions(tm PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(tm PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)


