message("Build rtc_pc static library")


file(GLOB rtc_pc_srcs *.h *.cc)

source_group(pc FILES ${rtc_pc_srcs})

list(FILTER  rtc_pc_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_pc_srcs EXCLUDE REGEX "peer_connection_wrapper.cc")

add_library(rtc_pc OBJECT ${rtc_pc_srcs} )

target_include_directories(rtc_pc PUBLIC ..)
target_include_directories(rtc_pc PUBLIC ../../libsrtp/include)
target_include_directories(rtc_pc PUBLIC ../../libsrtp/config)
target_include_directories(rtc_pc PUBLIC ../../libsrtp/crypto/include)
target_include_directories(rtc_pc PUBLIC ../../abseil-cpp/)
target_include_directories(rtc_pc PUBLIC ../../boringssl/include/)

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_pc PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_pc PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_pc PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

