message("Build pcclient server...")

file(GLOB pc_client_src 
        linux/main.cc
        linux/main_wnd.cc
        peer_connection_client.cc 
        conductor.cc
        defaults.cc
        )

link_directories(../../../boringssl/build/ssl)
link_directories(../../../boringssl/build/crypto)

source_group(source FILES ${pc_client_src})
add_executable(pcclient ${pc_client_src})
target_link_libraries(pcclient rtc_p2p rtc_base ssl crypto)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLIB REQUIRED glib-2.0)
target_include_directories(pcclient PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(pcclient INTERFACE ${GLIB_LDFLAGS})

pkg_search_modules (GTK2 REQUIRED gtk+-2.0)
target_include_directories (pcclient PRIVATE ${GTK2_INCLUDE_DIRS})
target_link_libraries(pcclient PRIVATE ${GTK2_LIBRARY_DIRS})

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
	target_compile_options(pcclient PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
	target_link_libraries(pcclient "${CMAKE_THREAD_LIBS_INIT}")
endif()


if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(pcclient PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(pcclient PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(pcclient PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)


