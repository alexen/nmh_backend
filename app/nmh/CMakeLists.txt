set(LIBNAME nmh)

add_library(${LIBNAME})

target_sources(
     ${LIBNAME}
     PRIVATE
          nmh.cpp
     PUBLIC
          nmh.h
)
target_include_directories(
     ${LIBNAME}
     PRIVATE
          ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
     ${LIBNAME}
     PRIVATE
          Boost::log
)

install(TARGETS ${LIBNAME})