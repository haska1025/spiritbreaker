message("Build rtc_api static library")


add_definitions(-DWEBRTC_APM_DEBUG_DUMP=0)
add_definitions(-DWEBRTC_USE_BUILTIN_ISAC_FLOAT)
add_definitions(-DWEBRTC_OPUS_VARIABLE_COMPLEXITY=0)

file(GLOB_RECURSE rtc_api_srcs *.h *.cc)

list(FILTER  rtc_api_srcs EXCLUDE REGEX ".*test.*")

source_group(api FILES ${rtc_api_srcs})

add_library(rtc_api OBJECT ${rtc_api_srcs})

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp) 

target_include_directories(rtc_api PRIVATE ../)
target_include_directories(rtc_api PRIVATE ../../opus/src/include/)
target_include_directories(rtc_api PRIVATE ../../libyuv/include/)
target_include_directories(rtc_api PRIVATE ../../abseil-cpp/)
target_include_directories(rtc_api PUBLIC ${JSONCPP_INCLUDE_DIRS})

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(rtc_api PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(rtc_api PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(rtc_api PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

