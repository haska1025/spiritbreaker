message("Build experiments static library")

add_definitions(-DWEBRTC_OPUS_SUPPORT_120MS_PTIME=1)

file(GLOB rtc_experiments_srcs 
	rate_control_settings.cc
        rate_control_settings.h
	rtt_mult_experiment.cc
	rtt_mult_experiment.h
	jitter_upper_bound_experiment.cc
	jitter_upper_bound_experiment.h
	normalize_simulcast_size_experiment.cc
	normalize_simulcast_size_experiment.h
	cpu_speed_experiment.cc
	cpu_speed_experiment.h
	field_trial_parser.cc
	field_trial_parser.h
	field_trial_units.cc
	field_trial_units.h
	alr_experiment.cc
	alr_experiment.h
	audio_allocation_settings.cc
	audio_allocation_settings.h
	)

source_group(source FILES ${rtc_experiments_srcs})

add_library(rtc_experiments OBJECT ${rtc_experiments_srcs})

target_include_directories(rtc_experiments PUBLIC ../..)
target_include_directories(rtc_experiments PUBLIC ../../../abseil-cpp/)

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

