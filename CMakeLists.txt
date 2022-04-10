cmake_minimum_required(VERSION 3.16)
message(STATUS "CMake version ${CMAKE_VERSION}")

project(HermesBDD VERSION 0.1)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.20)
        message(FATAL_ERROR "MSVC version must be at least 19.20!")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.2)
        message(FATAL_ERROR "GCC version must be at least 10.2!")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
        message(FATAL_ERROR "Clang version must be at least 11.0!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has
            only been tested with Clang, GCC and MSVC.")
endif()

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(LIBRARY_KIND STATIC)
else()
    set(LIBRARY_KIND SHARED)
endif()

set(CMAKE_MACOSX_RPATH 1)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT WIN32)
    set(ANY_TARGET_WARN extra pedantic sign-conversion cast-qual disabled-optimization
            init-self missing-include-dirs sign-promo switch-default undef redundant-decls
            strict-aliasing unused-parameter shadow error)
    foreach(WARN ${ANY_TARGET_WARN})
        add_compile_options(-W${WARN})
    endforeach()
else()
    add_compile_options(/WX)
endif()

find_package(Threads REQUIRED)

option(COVERAGE "Enable coverage reporting" OFF)
add_library(coverage_config INTERFACE)
if(COVERAGE)
    message(STATUS "Enabled coverage reporting")
    target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(coverage_config INTERFACE --coverage)
    else()
        target_link_libraries(coverage_config INTERFACE --coverage)
    endif()
endif()

option(NO_CACHE "Do not use cache" OFF)
if(NO_CACHE)
    MESSAGE(STATUS "NO_CACHE enabled")
    add_definitions(-DNO_CACHE)
endif()

option(NO_THREAD "Do not use thread" OFF)
if(NO_THREAD)
    MESSAGE(STATUS "NO_THREAD enabled")
    add_definitions(-DNO_THREAD)
endif()

enable_testing()
include(CTest)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/utility)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(profile)

add_library(hermesbdd ${LIBRARY_KIND} $<TARGET_OBJECTS:hermesbdd-all>)
target_link_libraries(hermesbdd ${CMAKE_THREAD_LIBS_INIT})

if(COVERAGE)
    target_link_libraries(hermesbdd coverage_config)
endif()
