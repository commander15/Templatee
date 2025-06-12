cmake_minimum_required(VERSION 3.30)

project(Templatee
    DESCRIPTION "Project generation commandline tool"
    VERSION     1.1.0
    LANGUAGES   CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(INJA_BUILD_TESTS OFF)
set(INJA_EXPORT      OFF)
set(INJA_INSTALL     OFF)
add_subdirectory(3rdparty/inja)

set(CMAKE_AUTOMOC ON)

add_executable(Tool main.cpp
    application.h application.cpp
    command/command.cpp command/command.h
    command/rendercommand.h command/rendercommand.cpp
    utils/jsonutils.h utils/jsonutils.cpp
    utils/renderer.h utils/renderer.cpp
    project/createproject.cpp project/createproject.h
    command/subcommand.h command/subcommand.cpp
)

set_target_properties(Tool PROPERTIES
    OUTPUT_NAME templatee
    RUNTIME_OUTPUT_DIRECTORY bin
)

target_link_libraries(Tool PRIVATE Qt::Core inja)

target_compile_definitions(Tool PRIVATE
    TOOL_VERSION="${PROJECT_VERSION}"
    TEMPLATES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/share/templates"
)

file(WRITE share/Templatee/version.txt ${PROJECT_VERSION})
file(COPY share/ DESTINATION ${CMAKE_BINARY_DIR}/share)

install(TARGETS Tool RUNTIME DESTINATION bin)
