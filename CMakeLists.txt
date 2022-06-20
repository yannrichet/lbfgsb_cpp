cmake_minimum_required(VERSION 3.10)
project(lbfgsb_cpp CXX)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_EXAMPLE "Build example" OFF)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(LBFGSB_CMAKE_GENERATOR "Unix Makefiles")
else ()
    set(LBFGSB_CMAKE_GENERATOR "${CMAKE_GENERATOR}")
    set(LBFGSB_CMAKE_GENERATOR "Unix Makefiles") # DEBUG
endif ()

# Did not use FetchContent since generated CMakeLists.txt doesn't support a different generator for the external project
set(lbfgsb_BINARY_DIR "${CMAKE_BINARY_DIR}/lbfgsb-build")
set(lbfgsb_INSTALL_DIR "${CMAKE_BINARY_DIR}/lbfgsb-installed")

# subbuild for lbfgsb
set(lbfgsb_VIRTUAL_PROJECT "${CMAKE_CURRENT_BINARY_DIR}/lbfgsb-virtual-project")
configure_file("${CMAKE_SOURCE_DIR}/cmake/ExternalFortranProject.cmake.in"
        "${lbfgsb_VIRTUAL_PROJECT}/CMakeLists.txt")
execute_process(
        COMMAND ${CMAKE_COMMAND}
        -DCMAKE_Fortran_COMPILER=${CMAKE_Fortran_COMPILER}
        -DFortran_LINK_FLAGS="${Fortran_LINK_FLAGS}"
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${lbfgsb_INSTALL_DIR}
        .
        RESULT_VARIABLE result
        OUTPUT_VARIABLE capturedOutput
        ERROR_VARIABLE capturedOutput
        WORKING_DIRECTORY "${lbfgsb_VIRTUAL_PROJECT}"
)
if (result)
    message("${capturedOutput}")
    message(FATAL_ERROR "CMake step for lbfgsb-build failed: ${result}")
endif ()
execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        OUTPUT_VARIABLE capturedOutput
        ERROR_VARIABLE capturedOutput
        WORKING_DIRECTORY "${lbfgsb_VIRTUAL_PROJECT}"
)
if (result)
    message("${capturedOutput}")
    message(FATAL_ERROR "Build step for lbfgsb-build failed: ${result}")
endif ()
# End of subbuild for lbfgsb

set(lbfgsb_DIR "${lbfgsb_INSTALL_DIR}/lib/cmake/lbfgsb")
find_package(lbfgsb CONFIG REQUIRED)

set(HEADERS "include/${PROJECT_NAME}/lbfgsb.hpp")

add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#target_link_libraries(${PROJECT_NAME} lbfgsb::lbfgsb)

target_include_directories(
        ${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

if (TARGET blas)
    target_link_libraries(${PROJECT_NAME} PUBLIC blas)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Release")
else ()
    #    target_compile_options(${PROJECT_NAME} PRIVATE -fcheck=all)
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
