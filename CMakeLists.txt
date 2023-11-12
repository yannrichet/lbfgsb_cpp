cmake_minimum_required(VERSION 3.10)
project(lbfgsb_cpp)

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_EXAMPLE "Build example" OFF)

set(HEADERS "include/${PROJECT_NAME}/lbfgsb.hpp")
add_library(${PROJECT_NAME} Lbfgsb.3.0/include/lbfgsb/lbfgsb.h
        Lbfgsb.3.0/blas.c
        Lbfgsb.3.0/lbfgsb.c
        Lbfgsb.3.0/linpack.c
        Lbfgsb.3.0/s_cmp.c
        Lbfgsb.3.0/s_copy.c
        Lbfgsb.3.0/timer.c
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(EMBED_CUSTOM_BLAS ON)
#if (TARGET blas)
#    set(EMBED_CUSTOM_BLAS OFF)
#    # target_link_libraries(${PROJECT_NAME} INTERFACE blas)
#endif ()

# Did not use FetchContent since generated CMakeLists.txt doesn't support a different generator for the external project
set(lbfgsb_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lbfgsb.3.0")
set(lbfgsb_BINARY_DIR "${CMAKE_BINARY_DIR}/lbfgsb-build")
set(lbfgsb_INSTALL_DIR "${CMAKE_BINARY_DIR}/lbfgsb-installed")
set(lbfgsb_DIR "${lbfgsb_INSTALL_DIR}/lib/cmake/lbfgsb")

target_include_directories(
        ${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    # Nothing to do
else ()
    # target_compile_options(${PROJECT_NAME} PRIVATE -fcheck=all)
endif ()

if (BUILD_EXAMPLE)
    enable_testing()
    add_subdirectory(examples)
endif ()

# Create install rule for your library. Use variables CMAKE_INSTALL_*DIR defined in GNUInstallDir
include(GNUInstallDirs)
# https://cmake.org/cmake/help/latest/command/install.html
# https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT devel
        )

install(FILES ${HEADERS} DESTINATION "include/${PROJECT_NAME}")

install(
        EXPORT ${PROJECT_NAME}_targets
        FILE ${PROJECT_NAME}Config.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
)

install(DIRECTORY
        ${lbfgsb_INSTALL_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX})