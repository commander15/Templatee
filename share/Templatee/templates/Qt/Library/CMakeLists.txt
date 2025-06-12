## set Qt.version = versionobject(Qt.version)
## set LocalesVar = "${" + Project.macro + "_LOCALES}"
cmake_minimum_required(VERSION {{ CMake.version }})

project({{ Project.name }}
    DESCRIPTION "{{ Project.description }}"
    VERSION     "{{ Project.version.string }}"
    LANGUAGES   CXX C
)

# Project settings

#option({{ Project.macro }}_BUILD_QML          "Build {{ Project.name }} QML module" OFF)
#option({{ Project.macro }}_BUILD_WIDGETS      "Build {{ Project.name }} widgets library" OFF)
option({{ Project.macro }}_BUILD_TESTS        "Build {{ Project.name }} tests" OFF)
option({{ Project.macro }}_BUILD_DOCS         "Build {{ Project.name }} documentation" OFF)
option({{ Project.macro }}_BUILD_TRANSLATIONS "Build {{ Project.name }} translations" OFF)
option({{ Project.macro }}_BUILD_EXAMPLES     "Build {{ Project.name }} Examples" OFF)

set({{ Project.macro }}_LOCALES {{ join(Project.locales, " ") }})

# CMake options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# CXX Standard
set(CMAKE_CXX_STANDARD {{ CXX.version }})
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# C Standard
set(CMAKE_C_STANDARD {{ C.version }})
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Registering project module path
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Including utility scripts
include(cmake/Utils.cmake)

# Qt setup

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

set(QT_MODULES {{ join(Qt.modules, " ") }})

if ({{ Project.macro }}_BUILD_QML)
    list(APPEND QT_MODULES Qml)
endif()

if ({{ Project.macro }}_BUILD_WIDGETS)
    list(APPEND QT_MODULES Widgets)
endif()

if ({{ Project.macro }}_BUILD_TESTS)
    list(APPEND QT_MODULES Test)
endif()

if ({{ Project.macro }}_BUILD_TRANSLATIONS)
    list(APPEND QT_MODULES LinguistTools)
endif()

find_package(Qt{{ Qt.version.major }} REQUIRED COMPONENTS ${QT_MODULES})

qt_standard_project_setup(
    REQUIRES {{ Qt.version.string }}
    I18N_SOURCE_LANGUAGE      en
    I18N_TRANSLATED_LANGUAGES {{ LocalesVar }}
)

## for policy in Qt.policies
qt_policy(SET {{ policy }} NEW)
## endfor

if ({{ Project.name }}_BUILD_QML)
    qt_policy(SET QTP0001 NEW)
    qt_policy(SET QTP0005 NEW)
endif()

# Other dependencies
## for dependency in Project.dependencies
find_package({{ dependency.name }}{% if dependency.required %} REQUIRED{% endif %})
## endfor

# Project directories

add_subdirectory(src)

if ({{ Project.macro }}_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if ({{ Project.macro }}_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if ({{ Project.macro }}_BUILD_TRANSLATIONS)
    qt_collect_translation_source_targets(SOURCE_TARGETS)

    qt_add_translations({{ Project.name }} SOURCE_TARGETS ${SOURCE_TARGETS}
        TS_FILE_BASE {{ Project.micro }}
        TS_FILE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/translations
        QM_FILES_OUTPUT_VARIABLE QM_FILES
    )
    
    # Ensure the build directory has a translations folder
    set(TRANSLATIONS_BUILD_DIR "${CMAKE_BINARY_DIR}/translations")
    file(MAKE_DIRECTORY ${TRANSLATIONS_BUILD_DIR})

    # Copy QM files to build directory after they are generated
    add_custom_command(
        TARGET {{ Project.name }}_lrelease POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} ${TRANSLATIONS_BUILD_DIR}
        COMMENT "Copying generated QM files to ${TRANSLATIONS_BUILD_DIR}"
    )

    install(FILES ${QM_FILES} DESTINATION translations)
endif()

if ({{ Project.macro }}_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
