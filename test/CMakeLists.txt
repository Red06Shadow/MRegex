set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/debug)

add_executable(regex_test test1.cpp)
target_link_libraries(regex_test PRIVATE myregex)

add_executable(converter_test test2.cpp)
target_link_libraries(converter_test PRIVATE myregex)

add_test(NAME regex_basic COMMAND regex_test)
add_test(NAME converter_unicode COMMAND converter_test)