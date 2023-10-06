set(THIS nmh)

add_library(${THIS})

target_sources(
     ${THIS}
     PRIVATE
          nmh.cpp
     PUBLIC
          nmh.h
)
target_include_directories(
     ${THIS}
     PRIVATE
          ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
     ${THIS}
     PRIVATE
          Boost::log
)

if(BUILD_TESTING)
     set(UNIT_TEST ${THIS}-test)
     add_executable(${UNIT_TEST})
     target_sources(
          ${UNIT_TEST}
          PRIVATE
               test/nmh_test.cpp
     )
     target_link_libraries(
          ${UNIT_TEST}
          PRIVATE
               ${THIS}
               Boost::unit_test_framework
     )
     add_test(${THIS} ${UNIT_TEST})
endif()

install(TARGETS ${THIS})