cmake_minimum_required(VERSION 3.5)

project(ga_view)

# set the module path for "findLib.cmake"
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

# add a "output" directory in the project binary directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# require Eigen
find_package(Eigen3 REQUIRED)
if(${EIGEN3_FOUND})
    message(STATUS "lib EIGEN3 found")
    message(STATUS "  version " ${EIGEN3_VERSION_STRING})
    message(STATUS "  include " ${EIGEN3_INCLUDE_DIR})
endif()

# require lib c3ga
find_package(C3GA)
if(${C3GA_FOUND})
    message(STATUS "lib C3GA found")
    message(STATUS "  include " ${C3GA_INCLUDE_DIRS})
    message(STATUS "  lib     " ${C3GA_LIBRARIES})
else()
    message(STATUS "lib C3GA should be set manually, please uncomment the commented lines in the CMakeLists.txt file")
    #Windows:
    #set(C3GA_INCLUDE_DIRS "D:/Partage/linux/garamon-master/build_WIN/Release/output/garamon_c3ga/src/")
    #set(C3GA_LIBRARIES "D:/Partage/linux/garamon-master/build_WIN/Release/output/garamon_c3ga/build/Release/c3ga.lib")

    #WSL
    set(C3GA_INCLUDE_DIRS "/mnt/d/Partage/linux/garamon-master/build/output/garamon_c3ga/src/")
    set(C3GA_LIBRARIES "/mnt/d/Partage/linux/garamon-master/build/output/garamon_c3ga/build/libc3ga.dylib")
    set(C3GA_LIBRARIES "/mnt/d/Partage/linux/garamon-master/build/output/garamon_c3ga/build/libc3ga.so")
    
    #default
    #set(C3GA_INCLUDE_DIRS "/usr/local/include/c3ga/")
    #set(C3GA_INCLUDE_DIRS "/usr/local/include/c3ga/")    
    #set(C3GA_LIBRARIES "/usr/local/lib/libc3ga.so")




    # for manual setting, check if valid
    if(EXISTS "${C3GA_INCLUDE_DIRS}")
        message(STATUS "include for lib c3ga found: ${C3GA_INCLUDE_DIRS}" )
    else()
        message(FATAL_ERROR "include for lib c3ga invalid or not found: ${C3GA_INCLUDE_DIRS}" )
    endif()

    # for manual setting, check if valid
    if(EXISTS "${C3GA_LIBRARIES}")
        message(STATUS "library c3ga found: ${C3GA_LIBRARIES}" )
    else()
        message(FATAL_ERROR "library c3ga not found: ${C3GA_LIBRARIES}" )
    endif()
endif()



# compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp -O2 -std=c++14")
#set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

# files to compile
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)
message(STATUS "  files " ${source_files})

# compile
add_executable(ga_view ${source_files})
#add_executable(ga_view src/Directory.cpp src/Directory.hpp src/Entry.hpp src/Entry.cpp src/main.cpp )

# includes
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${C3GA_INCLUDE_DIRS})

# link
#target_link_libraries(ga_view ${C3GA_LIBRARIES} )


