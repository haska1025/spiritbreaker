message("Build rtc_logging static library")

file(GLOB_RECURSE rtc_logging_srcs  *.h *.cc)

source_group(source FILES ${rtc_logging_srcs})

list(FILTER  rtc_logging_srcs EXCLUDE REGEX ".*test.*")
list(FILTER  rtc_logging_srcs EXCLUDE REGEX ".*mock.*")

find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()

message("protobuf path: ${PROTOBUF_INCLUDE_DIRS}")
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
        rtc_event_log/rtc_event_log.proto
        rtc_event_log/rtc_event_log2.proto
        )

add_library(rtc_logging OBJECT ${rtc_logging_srcs} ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(rtc_logging PUBLIC ../)
target_include_directories(rtc_logging PUBLIC ../../abseil-cpp/)

add_definitions(-DWEBRTC_ENABLE_PROTOBUF)
if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_logging PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_logging PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_logging PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

