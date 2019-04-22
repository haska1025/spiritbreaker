message("Build rtc_audiocodecs static library")

file(GLOB rtc_audiocodecs_srcs 
        audio_codec_pair_id.cc
        audio_decoder.cc
        audio_encoder.cc
        audio_format.cc
        builtin_audio_decoder_factory.cc
        builtin_audio_encoder_factory.cc
        isac/audio_decoder_isac_float.cc
        isac/audio_encoder_isac_float.cc
        g722/audio_decoder_g722.cc
        g722/audio_encoder_g722.cc
        ilbc/audio_decoder_ilbc.cc
        ilbc/audio_encoder_ilbc.cc
        g711/audio_decoder_g711.cc
        g711/audio_encoder_g711.cc
        L16/audio_decoder_L16.cc
        L16/audio_encoder_L16.cc

#opus/audio_decoder_opus.cc
#        opus/audio_encoder_opus.cc
#        opus/audio_encoder_opus_config.cc
	)

source_group(source FILES ${rtc_audiocodecs_srcs})

add_library(rtc_audiocodecs OBJECT ${rtc_audiocodecs_srcs})

target_include_directories(rtc_audiocodecs PUBLIC ../..)
target_include_directories(rtc_audiocodecs PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_audiocodecs PUBLIC ../../../boringssl/src/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_audiocodecs PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_audiocodecs PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_audiocodecs PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
    add_definitions(-DWEBRTC_USE_BUILTIN_ISAC_FLOAT)
endif(WIN32)

