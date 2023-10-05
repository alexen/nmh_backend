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

if(BUILD_TESTING)
     set(LIB_TEST ${LIBNAME}-test)
     add_executable(${LIB_TEST})
     target_sources(
          ${LIB_TEST}
          PRIVATE
               test/nmh_test.cpp
     )
     target_link_libraries(
          ${LIB_TEST}
          PRIVATE
               ${LIBNAME}
               Boost::unit_test_framework
     )
     add_test(${LIBNAME} ${LIB_TEST})
endif()

install(TARGETS ${LIBNAME})