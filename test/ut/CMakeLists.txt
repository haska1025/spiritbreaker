message("Build spiritbreaker ut test...")

cmake_minimum_required(VERSION 2.8.11)

file(GLOB ut_srcs *.cpp *.h)

source_group(source FILES ${ut_srcs})


link_directories(../../src/third_party/boringssl/build/ssl)
link_directories(../../src/third_party/boringssl/build/crypto)
link_directories(../../src/third_party/libsrtp/)
link_directories(../../src/third_party/libjpeg/build)
link_directories(../../src/third_party/libyuv/build)
link_directories(../../src/third_party/libvpx/source/libvpx)
link_directories(../../src/third_party/rnnoise/build)

link_directories(../../src/third_party/abseil-cpp/build/absl/strings)
link_directories(../../src/third_party/abseil-cpp/build/absl/base)
link_directories(../../src/third_party/abseil-cpp/build/absl/types/)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
pkg_check_modules(PROTOBUF protobuf)
pkg_check_modules(CPPUNIT cppunit)

add_executable(sbs_ut ${ut_srcs})

target_include_directories(sbs_ut PUBLIC ../../src/third_party/webrtc/)
target_include_directories(sbs_ut PUBLIC ../../src/third_party/abseil-cpp/)
target_include_directories(sbs_ut PUBLIC ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(sbs_ut sbs 
        webrtc
        yuv jpeg
        vpx rnnoise
        srtp2 ssl crypto 
        absl_bad_optional_access absl_strings absl_base absl_throw_delegate absl_bad_variant_access
        ${JSONCPP_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
        -ldl
        )

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(sbs_ut PUBLIC "-pthread")
    endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(sbs_ut "${CMAKE_THREAD_LIBS_INIT}")
endif()

if(WIN32)
	add_definitions(-DWEBRTC_WIN)
	target_link_libraries(sbs_ut PRIVATE wsock32 ws2_32 secur32 wininet Shlwapi)
	# require min win system version xp (0x0501)
	target_compile_definitions(sbs_ut PRIVATE "_WIN32_WINNT=0x0501" PRIVATE "WIN32_LEAN_AND_MEAN" PRIVATE "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS")
	# Generate map file and assembler output
	set_target_properties(sbs_ut PROPERTIES COMPILE_FLAGS "/FAcs" LINK_FLAGS "/MAP") 
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
	add_definitions(-DWEBRTC_POSIX)
	add_definitions(-DWEBRTC_LINUX)
endif(WIN32)

