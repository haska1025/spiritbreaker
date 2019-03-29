message("Build spiritbreaker...")

cmake_minimum_required(VERSION 2.8.11)

file(GLOB sbs_srcs *.cpp *.h *.cc)

source_group(source FILES ${sbs_srcs})

add_executable(sbs ${sbs_srcs})
link_directories(../../build/thirdparty/webrtc/rtc_base/)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)


target_include_directories(sbs PUBLIC ../thirdparty/webrtc/)
target_include_directories(sbs PUBLIC ../thirdparty/abseil-cpp/)
target_include_directories(sbs PUBLIC ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(sbs rtc_base ${JSONCPP_LIBRARIES})

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(sbs PUBLIC "-pthread")
    endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(sbs "${CMAKE_THREAD_LIBS_INIT}")
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

