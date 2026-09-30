# include(FetchContent)
# # 2. Declarar GoogleTest
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG v1.14.0  # Usa una versión estable
# )

# # 3. Descargar y añadir al proyecto
# FetchContent_MakeAvailable(googletest)

add_executable(tests test1.cpp)
target_link_libraries(tests
    myregex
    # gtest_main
    # gtest
)
# include(GoogleTest)
# gtest_discover_tests(tests)