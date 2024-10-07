set(THIS mt_queue_usage)
add_executable(${THIS}
     mt_queue_usage.cpp
)
target_link_libraries(
     ${THIS}
     PRIVATE
          Boost::log
          Boost::thread
)
