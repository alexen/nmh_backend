cmake_minimum_required(VERSION 3.22)

project(nmh_backend
     VERSION 1.0.0
     DESCRIPTION "NMH backend for testing"
     LANGUAGES CXX
)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
     set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.local/${PROJECT_NAME} CACHE PATH "" FORCE)
endif()

option(BUILD_SHARED_LIBS "Build shared libs by default" ON)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -fno-inline")

include(GNUInstallDirs)

set(CMAKE_INSTALL_RPATH $ORIGIN/../${CMAKE_INSTALL_LIBDIR})

find_package(Boost REQUIRED
     COMPONENTS
          log
          log_setup
          iostreams
          unit_test_framework
          thread
)

add_compile_options(-Wall -Werror -pedantic)

include(CTest)

add_subdirectory("extension")
add_subdirectory("app")

install(TARGETS ${PROJECT_NAME})