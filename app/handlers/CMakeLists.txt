set(THIS "handlers")
add_library(${THIS})
target_sources(
     ${THIS}
     PRIVATE
          echo.cpp
     PUBLIC
          echo.h
)
target_link_libraries(
     ${THIS}
     PRIVATE
          Boost::log
)
install(TARGETS ${THIS})