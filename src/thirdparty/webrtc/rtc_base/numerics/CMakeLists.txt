message("Build experiments static library")

file(GLOB rtc_numerics_srcs
	sample_counter.cc
	sample_counter.h
	samples_stats_counter.cc
	samples_stats_counter.h
	sequence_number_util.h
	moving_max_counter.h
	moving_median_filter.h
	percentile_filter.h
	safe_compare.h
	safe_conversions.h
	safe_conversions_impl.h
	safe_minmax.h
	exp_filter.h
	histogram_percentile_counter.cc
	histogram_percentile_counter.h
	math_utils.h
	mod_ops.h
	moving_average.cc
	moving_average.h
	exp_filter.cc
	)

source_group(source FILES ${rtc_numerics_srcs})

add_library(rtc_numerics OBJECT ${rtc_numerics_srcs})

target_include_directories(rtc_numerics PUBLIC ../..)
target_include_directories(rtc_numerics PUBLIC ../../../abseil-cpp/)

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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

