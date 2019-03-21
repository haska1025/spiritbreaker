message("Build systemwrapper static library")

file(GLOB rtc_systemwrappers_srcs 
	./source/cpu_features_linux.c
	./source/cpu_info.cc
	./source/event.cc
	./source/field_trial.cc
	./source/metrics.cc
	./source/rtp_to_ntp_estimator.cc
	./source/sleep.cc
	./source/clock.cc
	./source/cpu_features.cc
	)

source_group(source FILES ${rtc_systemwrappers_srcs})

add_library(rtc_systemwrappers ${rtc_systemwrappers_srcs})

target_include_directories(rtc_systemwrappers PUBLIC ..)
target_include_directories(rtc_systemwrappers PUBLIC ../../abseil-cpp/)
target_include_directories(rtc_systemwrappers PUBLIC ../../boringssl/src/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
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

