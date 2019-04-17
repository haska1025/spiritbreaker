message("Build rtc_p2p static library")


file(GLOB_RECURSE rtc_p2p_srcs *.h *.cc)

source_group(p2p FILES ${rtc_p2p_srcs})

list(FILTER  rtc_p2p_srcs EXCLUDE REGEX ".*test.*")

add_library(rtc_p2p OBJECT ${rtc_p2p_srcs}) 

target_include_directories(rtc_p2p PUBLIC ..)
target_include_directories(rtc_p2p PUBLIC ../../abseil-cpp/)


if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_p2p PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_p2p PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_p2p PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)


