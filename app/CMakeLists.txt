add_subdirectory("nmh")
add_subdirectory("tools")

set(BINARY_NAME ${PROJECT_NAME})

add_executable(${BINARY_NAME})

target_sources(
     ${BINARY_NAME}
     PRIVATE
          main.cpp
)

target_include_directories(
     ${BINARY_NAME}
     PRIVATE
          ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
     ${BINARY_NAME}
     PRIVATE
          nmh
          tools
          Boost::log_setup
)

# Подавляет ругань при компиляции:
#   /usr/include/boost/bind.hpp:36:1: note:
#     ‘#pragma message: The practice of declaring the Bind placeholders (_1, _2, ...) in the global namespace
#     is deprecated. Please use <boost/bind/bind.hpp> + using namespace boost::placeholders, or define
#     BOOST_BIND_GLOBAL_PLACEHOLDERS to retain the current behavior.’
target_compile_definitions(${PROJECT_NAME}
     PRIVATE
          BOOST_ALLOW_DEPRECATED_HEADERS
          BOOST_BIND_GLOBAL_PLACEHOLDERS
)

set(TARGET_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.json)

configure_file(
     manifest.json.in
     ${TARGET_FILE}
     @ONLY
)

install(
     FILES ${TARGET_FILE}
     DESTINATION $ENV{HOME}/.config/google-chrome/NativeMessagingHosts/
)
