set(THIS "tools")

add_library(${THIS})

target_sources(
     ${THIS}
     PRIVATE
          logger/init.cpp
          converter/hexlify.cpp
     PUBLIC
          logger/init.h
          converter/hexlify.h
)
target_link_libraries(
     ${THIS}
     PRIVATE
          Boost::log_setup
          Boost::thread
)

add_subdirectory("examples")

if(BUILD_TESTING)
     set(UNIT_TEST ${THIS}-test)
     add_executable(${UNIT_TEST})
     target_sources(
          ${UNIT_TEST}
          PRIVATE
               tests/hexlify_test.cpp
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