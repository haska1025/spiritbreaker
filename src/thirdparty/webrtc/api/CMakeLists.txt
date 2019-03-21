message("Build rtc_api static library")

set(rtc_api_VERSION 2.0.0)

add_subdirectory(crypto)
#add_subdirectory(ortc)
#add_subdirectory(audio)
#add_subdirectory(audio_codecs)
#add_subdirectory(video)
#add_subdirectory(video_codecs)
#add_subdirectory(transport)
#add_subdirectory(task_queue)

file(GLOB rtc_api_srcs 
	candidate.cc
	)

source_group(source FILES ${rtc_api_srcs})

add_library(rtc_api ${rtc_api_srcs})

target_include_directories(rtc_api PUBLIC ..)
target_include_directories(rtc_api PUBLIC ../../abseil-cpp/)
target_include_directories(rtc_api PUBLIC ../../boringssl/src/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(tm PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(tm PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(tm PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
	# Instruct CMake to run moc automatically when needed.
	# set_target_properties(tm PROPERTIES MACOSX_RPATH OFF AUTOMOC ON)

	# Find the Qt5Network library
	# find_package(Qt5Network REQUIRED)
	# target_link_libraries(tm PRIVATE Qt5::Network)

endif(WIN32)

