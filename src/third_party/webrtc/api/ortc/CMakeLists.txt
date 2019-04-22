message("Build rtc_ortc static library")

file(GLOB rtc_ortc_srcs 
	crypto_options.cc
	)

source_group(source FILES ${rtc_ortc_srcs})

add_library(rtc_ortc OBJECT ${rtc_ortc_srcs})

target_include_directories(rtc_ortc PUBLIC ../..)
target_include_directories(rtc_ortc PUBLIC ../../../abseil-cpp/)
target_include_directories(rtc_ortc PUBLIC ../../../boringssl/src/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_ortc PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_ortc PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_ortc PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

