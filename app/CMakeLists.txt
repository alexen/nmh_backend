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
