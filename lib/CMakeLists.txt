message("Build spiritbreaker...")

cmake_minimum_required(VERSION 2.8.11)

project(sbs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
add_definitions(-DWEBRTC_POSIX)
add_definitions(-DWEBRTC_LINUX)
add_definitions(-DBUILDING_NODE_EXTENSION)

file(GLOB sbs_srcs ../src/sbs/*.h ../src/sbs/*.cpp)

source_group(source FILES ${sbs_srcs})


link_directories(../src/third_party/boringssl/src/build/ssl)
link_directories(../src/third_party/boringssl/src/build/crypto)
link_directories(../src/third_party/libsrtp/)
link_directories(../src/third_party/libjpeg/build)
link_directories(../src/third_party/libyuv/build)
link_directories(../src/third_party/libvpx/source/libvpx)
link_directories(../src/third_party/rnnoise/build)

link_directories(../src/third_party/abseil-cpp/build/absl/strings)
link_directories(../src/third_party/abseil-cpp/build/absl/base)
link_directories(../src/third_party/abseil-cpp/build/absl/types/)
link_directories(../build/src/third_party/webrtc/)
#link_directories(/home/haska/webrtc/src/out/Default/obj)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
pkg_check_modules(PROTOBUF protobuf)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_srcs})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC} node_modules/nan/)

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
#target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

target_include_directories(${PROJECT_NAME} PUBLIC /home/haska/webrtc/src)
target_include_directories(${PROJECT_NAME} PUBLIC /home/haska/webrtc/src/third_party/abseil-cpp/)
target_include_directories(${PROJECT_NAME} PUBLIC /home/haska/webrtc/src/third_party/jsoncpp/source/include/)
target_link_libraries(${PROJECT_NAME} 
        webrtc
        yuv jpeg
        vpx rnnoise
        srtp2 ssl crypto
        absl_bad_optional_access absl_strings absl_base absl_throw_delegate absl_bad_variant_access
        ${JSONCPP_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        -ldl
        )
##target_link_libraries(${PROJECT_NAME} 
#        webrtc
#        )


find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
    endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()


