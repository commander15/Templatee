enable_testing()

find_package(GTest REQUIRED)

add_executable({{ Project.name }}Test
    main.cpp
)

target_compile_definitions({{ Project.name }}Test PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/testdata")

target_link_libraries({{ Project.name }}Test PRIVATE GTest::gtest Qt::Test {{ Project.name }})
