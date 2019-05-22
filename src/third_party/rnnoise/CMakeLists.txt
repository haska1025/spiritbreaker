cmake_minimum_required(VERSION 2.8.11)
project(rnnoise)

message("Build rnnoise static library")

set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_definitions(-DWEBRTC_USE_BUILTIN_ISAC_FLOAT)

file(GLOB_RECURSE rnnoise_srcs *.h *.c *.cc)

list(FILTER  rnnoise_srcs EXCLUDE REGEX ".*test.*")

source_group(source FILES ${rnnoise_srcs})

add_library(rnnoise STATIC ${rnnoise_srcs})

target_include_directories(rnnoise PRIVATE ../../)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
