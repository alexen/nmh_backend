set(TARGET_DIR ${CMAKE_BINARY_DIR}/extension)
set(TARGET_MANIFEST_FILE ${TARGET_DIR}/manifest.json)
set(TARGET_JAVASCRIPT_FILE ${TARGET_DIR}/simon.js)

configure_file(manifest.json.in ${TARGET_MANIFEST_FILE} @ONLY)
configure_file(simon.js.in ${TARGET_JAVASCRIPT_FILE} @ONLY)
