cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)

set(CMAKE_CXX_COMPILER /usr/local/bin/zapcc++)
message("C++ compiler: " ${CMAKE_CXX_COMPILER})
if(${CMAKE_CXX_COMPILER} MATCHES "zapcc")
    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1/")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
    endif()
endif()

project(Concurrency)

macro(build_code name)
    add_executable(${name} listings/listing_${name}.cpp)    
endmacro()

#file(GLOB files "listings/*.cpp")
#foreach(file ${files})
#    string(REGEX MATCH "([^_]+)\.cpp$" name "${file}")
#    build_code(${CMAKE_MATCH_1})
#endforeach()

# run project here
build_code(1.1)
