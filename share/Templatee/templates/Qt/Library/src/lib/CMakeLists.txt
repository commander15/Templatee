qt_add_library({{ Project.name }})

set_target_properties({{ Project.name }} PROPERTIES
    OUTPUT_NAME {{ Project.name }}
    VERSION     ${PROJECT_VERSION}
)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers({{ Project.name }}
    PUBLIC
        {{ Project.micro }}.h
        global.h config.h
    PRIVATE
        debug_p.h
)

target_sources({{ Project.name }}
    PRIVATE
        debug.cpp
)

target_include_directories({{ Project.name }} 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/{{ Project.name }}>
)

target_compile_definitions({{ Project.name }}
    PUBLIC
        $<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>,{{ Project.macro }}_STATIC,{{ Project.macro }}_SHARED>
    INTERFACE
        {{ Project.macro }}_LIB
    PRIVATE
        {{ Project.macro }}_BUILD
)

target_link_libraries({{ Project.name }} PUBLIC {{ join(dependencymodules("Qt", "::", Qt.modules), " ") }})

install(TARGETS {{ Project.name }}
    EXPORT {{ Project.name }}Targets
    ARCHIVE        DESTINATION lib
    LIBRARY        DESTINATION lib
    RUNTIME        DESTINATION bin
    PUBLIC_HEADER  DESTINATION include/{{ Project.name }}
    #PRIVATE_HEADER DESTINATION include/{{ Project.name }}/private
)

generate_target_headers({{ Project.name }} FOLDER {{ Project.name }})
