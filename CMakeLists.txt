cmake_minimum_required(VERSION 3.10)
project(logger)

include(GenerateExportHeader)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS false)

add_library(logger "lib/logger.cpp" "include/logger.h" "include/enum.h")
generate_export_header(logger)
target_include_directories(logger PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                                  $<INSTALL_INTERFACE:include>)

set_target_properties(logger PROPERTIES PUBLIC_HEADER "include/logger.h;include/enum.h;${CMAKE_CURRENT_BINARY_DIR}/logger_export.h")

install(TARGETS logger EXPORT loggerConfig
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
       )

install(EXPORT loggerConfig DESTINATION lib/cmake/logger)

add_executable(tester "bin/tester.cpp")
target_link_libraries(tester logger)
if(NOT MSVC)
    find_package(Threads)
    target_link_libraries(tester ${CMAKE_THREAD_LIBS_INIT})
endif()
