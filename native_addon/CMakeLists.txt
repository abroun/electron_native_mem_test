cmake_minimum_required(VERSION 3.22)

project(native-addon LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(native-addon
    SHARED 
    addon.cpp
    ${CMAKE_JS_SRC})

target_include_directories(native-addon
    PUBLIC 
    ${CMAKE_JS_INC})

set_target_properties(native-addon PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "native-addon"
    SUFFIX ".node")

target_link_libraries(native-addon
    ${CMAKE_JS_LIB})

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR)
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(native-addon
    PRIVATE ${NODE_ADDON_API_DIR})

# if(WIN32)
# # Post build, make a copy of the CUDA DLLs that we use
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy ${CUDAToolkit_BIN_DIR}/nppial64_12.dll $<TARGET_FILE_DIR:eye-node-js>)
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy ${CUDAToolkit_BIN_DIR}/nppif64_12.dll $<TARGET_FILE_DIR:eye-node-js>)
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy ${CUDAToolkit_BIN_DIR}/nppist64_12.dll $<TARGET_FILE_DIR:eye-node-js>)

# set(FINAL_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Release/windows)
# else()
# # Set RPATH in Linux shared object so that it will look next to eye.node for shared libraries
# set_target_properties(eye-node-js
#   PROPERTIES LINK_FLAGS "-Wl,-rpath,'$ORIGIN' -Wl,--version-script=${CMAKE_SOURCE_DIR}/node.version")

# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy ${CUDAToolkit_LIBRARY_DIR}/libcudart.so.12 $<TARGET_FILE_DIR:eye-node-js>)

# set(FINAL_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Release/linux)
# endif()

# # Then move the output dir to be Release
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E echo "rm -rf ${CMAKE_BINARY_DIR}/Release")
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/Release)
#   add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E echo "make_directory ${CMAKE_BINARY_DIR}/Release")
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release)
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E echo "rename $<TARGET_FILE_DIR:eye-node-js> ${FINAL_OUTPUT_PATH}")
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE_DIR:eye-node-js> ${FINAL_OUTPUT_PATH})

# # Copy eye_server into place
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E echo "copy $<TARGET_FILE:eye::eye_server> ${FINAL_OUTPUT_PATH}")
# add_custom_command(TARGET eye-node-js POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eye::eye_server> ${FINAL_OUTPUT_PATH})
