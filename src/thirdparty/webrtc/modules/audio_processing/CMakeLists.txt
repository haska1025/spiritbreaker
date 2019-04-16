message("Build rtc_audio_processing static library")

file(GLOB rtc_audio_processing_srcs 
        include/audio_processing_statistics.cc
#        audio_buffer.cc
#        audio_processing_impl.cc
#        echo_cancellation_impl.cc
#        echo_control_mobile_impl.cc
#        gain_control_for_experimental_agc.cc
#        gain_control_impl.cc
#        gain_controller2.cc
#        level_estimator_impl.cc
#        low_cut_filter.cc
#        noise_suppression_impl.cc
#        noise_suppression_proxy.cc
#        residual_echo_detector.cc
#        rms_level.cc
#        splitting_filter.cc
#        three_band_filter_bank.cc
#        typing_detection.cc
#        voice_detection_impl.cc
)

source_group(source FILES ${rtc_audio_processing_srcs})

add_library(rtc_audio_processing ${rtc_audio_processing_srcs})

target_include_directories(rtc_audio_processing PUBLIC ../../)
target_include_directories(rtc_audio_processing PUBLIC ../../../abseil-cpp/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_audio_processing PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_audio_processing PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_audio_processing PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

