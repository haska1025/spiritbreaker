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

