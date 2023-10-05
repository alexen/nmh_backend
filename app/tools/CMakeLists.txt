set(THIS "tools")

add_library(${THIS})

target_sources(
     ${THIS}
     PRIVATE
          logger/init.cpp
     PUBLIC
          logger/init.h
          converter/hexlify.h
)
target_link_libraries(
     ${THIS}
     PRIVATE
          Boost::log_setup
)

install(TARGETS ${THIS})