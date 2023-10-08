set(THIS nmh)

add_library(${THIS})

target_compile_definitions(
     ${THIS}
     PRIVATE
          BOOST_BIND_GLOBAL_PLACEHOLDERS
)
target_sources(
     ${THIS}
     PRIVATE
          io.cpp
          request.cpp
          response.cpp
          workers.cpp
     PUBLIC
          io.h
          request.h
          response.h
          workers.h
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
               test/io_test.cpp
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