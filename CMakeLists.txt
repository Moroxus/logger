cmake_minimum_required(VERSION 3.0)
project(logger)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC "logger.cpp" "include/logger.h" "include/enum.h")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_strong_enums)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                  $<INSTALL_INTERFACE:include/logger>)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "include/logger.h;include/enum.h")

install(TARGETS ${PROJECT_NAME} EXPORT loggerConfig
        LIBRARY DESTINATION lib/logger
        ARCHIVE DESTINATION lib/logger
        PUBLIC_HEADER DESTINATION include/logger
       )
install(EXPORT loggerConfig DESTINATION lib/logger)

INCLUDE(CPack)

add_executable(tester "main.cpp")
if(NOT MSVC)
    find_package(Threads)
    target_link_libraries(tester ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(tester ${PROJECT_NAME})
