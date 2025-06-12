qt_add_library({{ Project.name }}Widgets)

set_target_properties({{ Project.name }}Widgets PROPERTIES
    OUTPUT_NAME {{ Project.name }}Widgets
    VERSION     ${PROJECT_VERSION}
)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers({{ Project.name }}Widgets
    PUBLIC
        {{ Project.micro }}-widgets.h
        global.h config.h
)

target_sources({{ Project.name }}Widgets
    PRIVATE
        global.h # Put your sources here instead !
)

target_include_directories({{ Project.name }}Widgets 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/{{ Project.name }}Widgets>
)

target_compile_definitions({{ Project.name }}Widgets
    INTERFACE
        {{ Project.macro }}_WIDGETS_LIB
    PRIVATE
        {{ Project.macro }}_WIDGETS_BUILD
)

target_link_libraries({{ Project.name }}Widgets PUBLIC Qt::Widgets {{ Project.name }})

install(TARGETS {{ Project.name }}Widgets
    EXPORT {{ Project.name }}Targets
    ARCHIVE        DESTINATION lib
    LIBRARY        DESTINATION lib
    RUNTIME        DESTINATION bin
    PUBLIC_HEADER  DESTINATION include/{{ Project.name }}Widgets
    #PRIVATE_HEADER DESTINATION include/{{ Project.name }}Widgets/private
)

generate_target_headers({{ Project.name }}Widgets FOLDER {{ Project.name }}Widgets)
