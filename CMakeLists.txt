cmake_minimum_required(VERSION 3.10...3.22)

# Set the project name and languages
project(dumpefs VERSION 0.1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -g -std=c++11")

# set(FMT_MODULE OFF)

# Set the source files
set(SOURCE_FILES 
   src/dumpefs.cpp
)

add_definitions(
   -D__LITTLEENDIAN__
   -D__LINUX__
   -D__STDC_LIB_EXT1__
   -D__STDC_WANT_LIB_EXT1__=1
   -D__cpp_modules
   -D__X86_64__
   -DGTHR_ACTIVE_PROXY
)

include_directories(
   ${CMAKE_SOURCE_DIR}/include  
   ${CMAKE_SOURCE_DIR}/spdlog-1.11.0/include
)
find_package(Threads REQUIRED)
# find_package(spdlog REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/spdlog-1.11.0)


# message("C++ Compiler Path: ${CMAKE_CXX_COMPILER}")
# message("C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
# message("C++ Compiler Options: ${CMAKE_CXX_FLAGS}")

# Add an executable target with the source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog )

# Set the required C++ standard
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Set the required C standard
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

target_compile_options(${PROJECT_NAME} PRIVATE )
# target_link_libraries(${PROJECT_NAME} PUBLIC libstdc++)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
