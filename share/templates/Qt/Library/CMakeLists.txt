## set Qt.version = versionobject(Qt.version)
## set LocalesVar = "${" + Project.macro + "_LOCALES}"
cmake_minimum_required(VERSION {{ CMake.version }})

project({{ Project.name }}
    DESCRIPTION "{{ Project.description }}"
    VERSION     "{{ Project.version.string }}"
    LANGUAGES   CXX C
)

# Project settings

option({{ Project.macro }}_BUILD_TESTS        OFF "Build {{ Project.name }} tests")
option({{ Project.macro }}_BUILD_DOCS         OFF "Build {{ Project.name }} documentation")
option({{ Project.macro }}_BUILD_TRANSLATIONS OFF "Build {{ Project.name }} translations")
option({{ Project.macro }}_BUILD_EXAMPLES     OFF "Build {{ Project.name }} Examples")

set({{ Project.macro }}_LOCALES {{ join(Project.locales, " ") }})

# CMake settings
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

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

set(Qt_MODULES {{ join(Qt.modules, " ") }})

if ({{ Project.macro }}_BUILD_TESTS)
    list(APPEND Qt_MODULES Test)
endif()

if ({{ Project.macro }}_BUILD_TRANSLATIONS)
    list(APPEND Qt_MODULES LinguistTools)
endif()

find_package(Qt{{ Qt.version.major }} REQUIRED COMPONENTS ${Qt_MODULES})

qt_standard_project_setup(
    REQUIRES {{ Qt.version.string }}
    I18N_SOURCE_LANGUAGE      en
    I18N_TRANSLATED_LANGUAGES {{ LocalesVar }}
)

## for policy in Qt.policies
qt_policy(SET {{ policy }} NEW)
## endfor

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
