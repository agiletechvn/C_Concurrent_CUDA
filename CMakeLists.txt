cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)

#set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9)

if(${CMAKE_CXX_COMPILER} MATCHES "zapcc")
    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1/")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
    endif()
endif()
message("C++ compiler: " ${CMAKE_CXX_COMPILER})

project(Concurrency)

find_package(Boost)

include_directories(/usr/local/Cellar/gcc/9.1.0/include/c++/9.1.0)
include_directories(/usr/local/opt/tbb/include)
link_directories(/usr/local/opt/tbb/lib)


macro(build_code name)
    add_executable(${name} listings/listing_${name}.cpp)        
    target_link_libraries(${name} ${Boost_LIBRARIES} tbb)
endmacro()

#file(GLOB files "listings/*.cpp")
#foreach(file ${files})
#    string(REGEX MATCH "([^_]+)\.cpp$" name "${file}")
#    if(${CMAKE_MATCH_1})
#        message("app: ${CMAKE_MATCH_1}")
#        build_code(${CMAKE_MATCH_1})
#    endif()
#endforeach()

build_code(4.13)
build_code(10.1)
