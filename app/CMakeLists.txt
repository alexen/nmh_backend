set(BINARY_NAME ${PROJECT_NAME})
add_executable(${BINARY_NAME} main.cpp)
target_link_libraries(${BINARY_NAME} Boost::log Boost::log_setup)

set(PROJECT_BROWSER_EXTENSION_ID "chrome-extension://hdljlmlimlofikgjfgbgbcolbkcgbbld/")

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
