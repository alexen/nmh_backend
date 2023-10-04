cmake_minimum_required(VERSION 3.22)

project(nmh_backend
     VERSION 1.0.0
     DESCRIPTION "NMH backend for testing"
     LANGUAGES CXX
)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
     set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.local CACHE PATH "" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -fno-inline")

include(GNUInstallDirs)

set(PROJECT_INSTALL_BINARY_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})

find_package(Boost REQUIRED
     COMPONENTS
          log
          log_setup
          iostreams
)

add_subdirectory("extension")
add_subdirectory("app")

install(TARGETS ${PROJECT_NAME})