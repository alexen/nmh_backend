set(TARGET_DIR ${CMAKE_BINARY_DIR}/extension)
set(TARGET_FILE ${TARGET_DIR}/manifest.json)

configure_file(manifest.json.in ${TARGET_FILE} @ONLY)
install(FILES simon.js DESTINATION ${TARGET_DIR})